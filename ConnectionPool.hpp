/* 
 * File:   ConnectionPool.h
 * Author: SChapel
 *
 * Created on January 6, 2011, 10:41 AM
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

