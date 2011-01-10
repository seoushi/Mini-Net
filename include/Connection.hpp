/* 
 * File:   Connection.h
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
        bool listen(int port);

        // accepts a new connection from a server connection
        Connection* accept();

        // connects to a remote server
        bool connect(const char* address, int port);

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
        void close();

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

