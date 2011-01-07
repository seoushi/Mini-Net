/* 
 * File:   main.cpp
 * Author: SChapel
 *
 * Created on January 5, 2011, 8:47 AM
 */

#include "Connection.hpp"
#include "ConnectionPool.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    ConnectionPool pool;

    Connection server;
    if(server.listen(7777))
    {
        std::cout << "Server is now listening on port: " << server.getAddress() << " : " << server.getPort();

        if(server.isIpv6())
        {
            std::cout << " (IPv6) " << std::endl;
        }
        else
        {
            std::cout << " (IPv4) " << std::endl;
        }

        pool.add(&server);
    }
    else
    {
        std::cout << "could not start listening!" << std::endl;
        return 1;
    }


    Connection* conPtr;
    int bufferSize = 128;
    char buffer[bufferSize];
    int bytesRead;

    for(;;)
    {
        pool.pollConnections();

        while(conPtr = pool.getNextReadyConnection())
        {
            if(conPtr->isServer())
            {
                Connection* newCon = conPtr->accept();
                std::cout << "got connection from: " << newCon->getAddress() << " : " << newCon->getPort() << std::endl;
                pool.add(newCon);
            }
            else
            {
                bytesRead = conPtr->read(buffer, bufferSize);

                // client disconnected?
                if(bytesRead <= 0)
                {
                    std::cout << "disconnection from: " << conPtr->getAddress() << " : " << conPtr->getPort() << std::endl;
                    
                    pool.remove(conPtr);
                    delete conPtr;
                }
                else // echo it back
                {
                    buffer[bytesRead] = 0; //terminate the end
                    conPtr->write(buffer, bytesRead);
                    std::cout << "echoed data: '" << buffer << "'" << std::endl;
                }
            }
        }
    }

    return 0;
}

