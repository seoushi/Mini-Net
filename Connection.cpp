/* 
 * File:   Connection.cpp
 * Author: SChapel
 * 
 * Created on January 5, 2011, 8:48 AM
 */

#include "Connection.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sstream>



Connection::Connection()
{
    port = 0;
    numBacklogConnections = 10;
    sockfd = NULL;
    isAServer = false;
    isIpv4 = true;
    address = "";
}

Connection::Connection(int sockfd, sockaddr* sa)
{
    numBacklogConnections = 10;
    this->sockfd = sockfd;
    isAServer = false;
    getSocketInfo(sa);
}

Connection::~Connection()
{
    close();
}


bool Connection::listen(int port)
{
    // convert port to string for usage
    std::stringstream portStr;
    portStr << port;

    // find a suitable address to bind to
    addrinfo hints;
    addrinfo* servinfo;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, portStr.str().c_str(), &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }


    // bind to the first available address
    addrinfo* si;
    int yes = 1;

    for(si = servinfo; si != NULL; si = si->ai_next)
    {
        if ((sockfd = socket(si->ai_family, si->ai_socktype, si->ai_protocol)) == -1)
        {
            perror("connection: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("connection: setsockopt");
            return false;
        }

        if (bind(sockfd, si->ai_addr, si->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("connection: bind");
            continue;
        }
        break;
    }

    if (si == NULL)
    {
        fprintf(stderr, "connection: failed to bind\n");
        return false;
    }
    //getSocketInfo(si->ai_addr);
    freeaddrinfo(servinfo);


    //start listening
    if(listen(sockfd, numBacklogConnections) == -1)
    {
        perror("connection: listen");
        return false;
    }

    this->address = "localhost";
    this->port = port;
    isAServer = true;

    return true;
}


Connection* Connection::accept()
{
    sockaddr_storage theirAddr;
    socklen_t sin_size = sizeof(theirAddr);

    int new_socketfd = accept(sockfd, (sockaddr*)&theirAddr, sin_size);

    if (new_socketfd == -1)
    {
        perror("acceptConnection");
        return NULL;
    }

    return new Connection(new_socketfd, (sockaddr*)&theirAddr);
}


bool Connection::connect(const char* address, int port)
{
    // convert port to string for usage
    std::stringstream portStr;
    portStr << port;

    // get list of addresses to try to connect to
    addrinfo hints;
    addrinfo* servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(address, portStr.str().c_str(), &hints, &servinfo);
    if(result != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        return false;
    }


    // try to connect to addresses
    addrinfo* si;

    for(si = servinfo; si != NULL; si = si->ai_next)
    {
        if((sockfd = socket(si->ai_family, si->ai_socktype, si->ai_protocol)) == -1)
        {
            perror("connect: socket");
            continue;
        }

        if(connect(sockfd, si->ai_addr, si->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("connect: connect");
            continue;
        }

        break;
    }
    

    if (si == NULL) {
        fprintf(stderr, "connect: failed to connect\n");
        return false;
    }
    getSocketInfo(si->ai_addr);
    freeaddrinfo(servinfo);

    
    // success!
    this->port = port;
    return true;
}


int Connection::read(char* buffer, int bufferSize)
{
    int bytesRead = recv(sockfd, buffer, bufferSize - 1, 0);

    if(bytesRead == -1)
    {
        perror("read");
        return 0;
    }

    return bytesRead;
}


bool Connection::write(const char* data, int length)
{
    int sent = 0;
    int bytesleft = length;
    int n;

    while(sent < length)
    {
        n = send(sockfd, data + sent, bytesleft, 0);
        if (n == -1)
        {
            perror("write");
            return false;
        }

        sent += n;
        bytesleft -= n;
    }

    return true;
}


bool Connection::isServer()
{
    return isAServer;
}


std::string Connection::getAddress()
{
    return address;
}

int Connection::getPort()
{
    return port;
}

bool Connection::isIpv6()
{
    return !isIpv4;
}

void Connection::close()
{
    if(sockfd)
    {
        close(sockfd);
    }

    sockfd = NULL;
}


void Connection::getSocketInfo(sockaddr* sa)
{
    char str[INET6_ADDRSTRLEN];

    void* sa_in;

    if (sa->sa_family == AF_INET)
    {
        isIpv4 = true;
        sa_in = &((sockaddr_in*)sa)->sin_addr;
        port = ((sockaddr_in*)sa)->sin_port;
    }
    else
    {
        isIpv4 = false;
        sa_in = &((sockaddr_in6*)sa)->sin6_addr;
        port = ((sockaddr_in6*)sa)->sin6_port;
    }

    inet_ntop(sa->sa_family, sa_in, str, sizeof str);

    address = std::string(str);
}

int Connection::accept(int sock, sockaddr* sockAddr, socklen_t sockLength)
{
    return accept(sock, sockAddr, sockLength);
}

int Connection::connect(int sock, sockaddr* sockAddr, socklen_t sockLength)
{
    return connect(sock, sockAddr, sockLength);
}

void Connection::close(int sock, bool phonyParameter)
{
    close(sock);
}

int Connection::listen(int sock, int numBacklogConnections)
{
    return listen(sock, numBacklogConnections);
}