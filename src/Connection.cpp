/* 
 * File:   Connection.cpp
 * 
 * Copyright © 2011, Sean Chapel
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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


/// wrappers arround c functions to get arround naming conflicts
int accept2(int sock, sockaddr* sockAddr, socklen_t sockLength);
int connect2(int sock, sockaddr* sockAddr, socklen_t sockLength);
void close2(int sock);
int listen2(int sock, int numBacklogConnections);




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

    if((rv = getaddrinfo(NULL, portStr.str().c_str(), &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }


    // bind to the first available address
    addrinfo* si;
    int yes = 1;
    int result;

    for(si = servinfo; si != NULL; si = si->ai_next)
    {
        sockfd = socket(si->ai_family, si->ai_socktype, si->ai_protocol);
        if(sockfd == -1)
        {
            perror("connection: socket");
            continue;
        }

        result =setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if(result == -1)
        {
            perror("connection: setsockopt");
            return false;
        }

        if(bind(sockfd, si->ai_addr, si->ai_addrlen) == -1)
        {
            close2(sockfd);
            perror("connection: bind");
            continue;
        }
        break;
    }

    if(si == NULL)
    {
        fprintf(stderr, "connection: failed to bind\n");
        return false;
    }
    //getSocketInfo(si->ai_addr);
    freeaddrinfo(servinfo);


    //start listening
    if(listen2(sockfd, numBacklogConnections) == -1)
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

    int new_socketfd = accept2(sockfd, (sockaddr*)&theirAddr, sin_size);

    if(new_socketfd == -1)
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
        sockfd = socket(si->ai_family, si->ai_socktype, si->ai_protocol);
        if(sockfd == -1)
        {
            perror("connect: socket");
            continue;
        }

        if(connect2(sockfd, si->ai_addr, si->ai_addrlen) == -1)
        {
            close2(sockfd);
            perror("connect: connect");
            continue;
        }

        break;
    }
    

    if(si == NULL) {
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
    int bytesRead = recv(sockfd, buffer, bufferSize, 0);

    if(bytesRead == -1)
    {
        perror("read error");
        return 0;
    }

    return bytesRead;
}


bool Connection::write(const char* data, int length)
{
    int sent = 0;
    int bytesLeft = length;
    int n;

    while(bytesLeft)
    {
        n = send(sockfd, data + sent, bytesLeft, 0);
        if(n == -1)
        {
            perror("write error");
            return false;
        }

        sent += n;
        bytesLeft -= n;
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
        close2(sockfd);
    }

    sockfd = NULL;
}


void Connection::getSocketInfo(sockaddr* sa)
{
    char str[INET6_ADDRSTRLEN];

    void* sa_in;

    if(sa->sa_family == AF_INET)
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

int accept2(int sock, sockaddr* sockAddr, socklen_t sockLength)
{
    return accept(sock, sockAddr, &sockLength);
}

int connect2(int sock, sockaddr* sockAddr, socklen_t sockLength)
{
    return connect(sock, sockAddr, sockLength);
}

void close2(int sock)
{
    close(sock);
}

int listen2(int sock, int numBacklogConnections)
{
    return listen(sock, numBacklogConnections);
}