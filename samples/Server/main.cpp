/*
 * File: main.cpp
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
#include "ConnectionPool.hpp"

#include <iostream>

int testServer();
int testDataBuffer();

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
