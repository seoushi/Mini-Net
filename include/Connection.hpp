/* 
 * File:   Connection.hpp
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

#include "DataBuffer.hpp"
#include "Message.hpp"

/**
 * The Connection class abstracts out the low level implementation for TCP/IP
 * sockets. Creates/connects to servers, reads/writes data.
 */
class Connection
{
    friend class ConnectionPool;

    public:

        /**
         *  Default constructor
         */
        Connection();

        /**
         * Constructor that uses an existing connection.
         * @param sockfd the socket
         * @param sa the socket address info
         */
        Connection(int sockfd, sockaddr* sa);

        /**
         *  Default destructor
         */
        ~Connection();

        /**
         * Starts a server on a specified port
         * @param port the port to use
         * @return true if succeeded otherwise false
         */
        bool listen(int port);

        /**
         * Accepts a new connection from a server connection
         * @return the new connection accepted
         */
        Connection* accept();

        /**
         * Connects to a remote server
         * @param address the address of the server
         * @param port the port of the server
         * @return true if connection succeeded otherwise false
         */
        bool connect(const char* address, int port);

        /**
         * Takes in a preallocated buffer and it's size
         * @param buffer the buffer to put the data into
         * @param bufferSize the size of the buffer
         * @return the number of bytes read
         */
        int read(char* buffer, int bufferSize);


        /**
         * reads data into a data buffer from it's current position and
         * filling up to it's allocated size
         * @param buffer the buffer to read into
         * @return the number of bytes read
         */
        int operator >>(DataBuffer& buffer);

        /**
         * reads data into a Message
         * @param msg the message to read into
         * @return if the message is done reading
         */
        bool operator >>(Message& msg);


        /**
         * Writes the data to the connection
         * @param data the data to write
         * @param length the length of the data
         * @return true if write succeeded otherwise false
         */
        bool write(const char* data, int length);

        /**
         * writes data from begining of the buffer into the connection up to
         * it's size
         * @param buffer the buffer to write to
         * @return a reference to the connection
         */
        Connection& operator <<(DataBuffer& buffer);

        /**
         * writes the message to the connection
         * @param msg the message to write to the buffer
         * @return a reference to the connection
         */
        Connection& operator <<(Message& msg);

        /**
         * Is the connection a server?
         * @return if the connection is a server
         */
	bool isServer();

        /**
         * Gets the address of the connection
         * @return the address of the connection
         */
        std::string getAddress();

        /**
         * Gets the port of the connection
         * @return the port of the connection
         */
        int getPort();

        /**
         * Is the connection over IPv6?
         * @return True if the connection is IPv6 and false for IPv4
         */
        bool isIpv6();

        /**
         * Closes the connection
         */
        void close();

    private:

        /**
         * Gets the address and port from a connection and stores it
         * @param sa the socket address info
         */
        void getSocketInfo(sockaddr* sa);

        int port;                   /**< The Port of the connection           */
        int numBacklogConnections;  /**< In a server the number of connections
                                     that can be waiting for an accept        */
        int sockfd;                 /**< The connection's socket              */
	bool isAServer;             /**< Is the connection a server?          */
        bool isIpv4;                /**< Is the connection IPv4?              */
        std::string address;        /**< The address of the connection        */
};

#endif	/* CONNECTION_H */

