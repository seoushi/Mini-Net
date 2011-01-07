/* 
 * File:   Connection.h
 * Author: SChapel
 *
 * Created on January 5, 2011, 8:48 AM
 */

#ifndef CONNECTION_H
#define	CONNECTION_H

class ConnectionPool;

#include <string>
#include <netdb.h>

class Connection
{
    friend class ConnectionPool;

    public:

        // default constructor
        Connection();

        // constructor that uses an existing connection
        Connection(int sockfd, sockaddr* sa);

        // default destructor
        ~Connection();

        // starts a server on a specified port
        bool makeServer(const char* port);

        // accepts a new connection from a server connection
        Connection* acceptConnection();

        // connects to a remote server
        bool connectToServer(const char* address, const char* port);

	// takes in a preallocated buffer and it's size
	// returns the number of bytes read
        int read(char* buffer, int bufferSize);

	// writes the data to the connection
        bool write(const char* data, int length);

        // returns if the connection is a server or not
	bool isServer();

        // gets the address of the connection
        std::string getAddress();

        // gets the port of the connection
        int getPort();

        // returns if the connection is over IPv6, otherwise it's IPv4
        bool isIpv6();

        // closes the connection
        void closeConnection();

    private:

        // stores the socket information (port and address)
        void getSocketInfo(sockaddr* sa);

        int port;
        int numBacklogConnections;
        int sockfd;
	bool isAServer;
        bool isIpv4;
        std::string address;
};

#endif	/* CONNECTION_H */

