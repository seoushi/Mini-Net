/* 
 * File:   ConnectionPool.cpp
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

#include "ConnectionPool.hpp"
#include <stdio.h>

ConnectionPool::ConnectionPool()
{
    FD_ZERO(&pool);
    FD_ZERO(&poolCopy);
    poolMax = 0;
    lastChecked = -1;
}


ConnectionPool::~ConnectionPool()
{
}


void ConnectionPool::add(Connection* con)
{
    connectionTable[con->sockfd] = con;
    FD_SET(con->sockfd, &pool);
    poolMax = con->sockfd;
}


void ConnectionPool::remove(Connection* con)
{
    connectionTable.erase(con->sockfd);
    FD_CLR(con->sockfd, &pool);
}


void ConnectionPool::pollConnections()
{
    poolCopy = pool;
    lastChecked = -1;

    timeval* timeoutVal = NULL;

    if(timeout.tv_usec >= 0)
    {
        timeoutVal = &timeout;
    }

    if(select(poolMax + 1, &poolCopy, NULL, NULL, timeoutVal) == -1)
    {
        perror("select");
    }
}


Connection* ConnectionPool::getNextReadyConnection()
{
    lastChecked++;

    if(lastChecked > poolMax)
    {
        return NULL;
    }


    for(;lastChecked <= poolMax; lastChecked++)
    {
        if(FD_ISSET(lastChecked, &poolCopy))
        {
            return connectionTable[lastChecked];
        }
    }

    return NULL;
}

void ConnectionPool::setPollTimeout(suseconds_t time)
{
    timeout.tv_usec = time;
    timeout.tv_sec = 0;
}