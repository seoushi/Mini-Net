/* 
 * File:   ConnectionPool.cpp
 * Author: SChapel
 * 
 * Created on January 6, 2011, 10:41 AM
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

void ConnectionPool::setPollTimeout(__suseconds_t time)
{
    timeout.tv_usec = time;
    timeout.tv_sec = 0;
}