/* 
 * File:   ConnectionPool.h
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

#ifndef CONNECTIONPOOL_H
#define	CONNECTIONPOOL_H

#include <map>
#include <time.h>

class ConnectionPool
{
    public:
        // default constructor
        ConnectionPool();

        // default destructor
        ~ConnectionPool();

        // adds a connection to the pool
        void add(Connection* con);

        // removes a connection from the pool
        void remove(Connection* con);

        // updates the connects and check for sockets that are ready to read data
        void pollConnections();

        // gets the next connection that is ready for reading
        // returns NULL when there are no other ready connections
        Connection* getNextReadyConnection();

        // sets the timeout time for polling sockets in microseconds (long int)
        // a negitive number will wait indefinitely
        void setPollTimeout(suseconds_t time);

    private:

        fd_set pool;
        fd_set poolCopy;
        int poolMax;
        int lastChecked;
        timeval timeout;

        std::map<int, Connection*> connectionTable; // maped from socket to connection
};

#endif	/* CONNECTIONPOOL_H */

