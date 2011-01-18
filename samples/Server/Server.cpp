/*
 * File: Server.cpp
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

#include "Server.hpp"



Server::Server(int port)
{
    this->port = port;
}


Server::~Server()
{

}


bool Server::start()
{
    if(con.listen(7777))
    {
        std::cout << "Server is now listening on port: " << con.address() << " : " << con.port();

        if(con.isIpv6())
        {
            std::cout << " (IPv6) " << std::endl;
        }
        else
        {
            std::cout << " (IPv4) " << std::endl;
        }

        pool.add(&con);

        return true;
    }


    std::cout << "could not start listening!" << std::endl;
    return false;
}


void Server::update()
{
    Connection* conPtr;

    pool.pollConnections();

    while(conPtr = pool.getNextReadyConnection())
    {
        if(conPtr->isServer())
        {
            Connection* newCon = conPtr->accept();
            onConnect(newCon);
            pool.add(newCon);
        }
        else
        {
            // are they connected
            if(!conPtr->isConnected())
            {
                onDisconnect(conPtr);
            }

            // see if the connection has an existing message
            std::map<Connection*, Message*>::iterator itr = msgBox.find(conPtr);

            Message* msg;

            if((itr == msgBox.end()) || (itr->second == NULL))
            {
                msg = new Message();
                msgBox[conPtr] = msg;
            }
            else
            {
                msg = itr->second;
            }


            // read message data and see if it's done
            if((*conPtr) >> (*msg))
            {
                onMessage(conPtr, msg);
                msgBox[conPtr] = NULL;
            }

            // are they still connected?
            if(!conPtr->isConnected())
            {
                onDisconnect(conPtr);
            }
        }
    }
}


void Server::onMessage(Connection* c, Message* msg)
{
    std::cout << "got Message from: " << c->address() << " : " << c->port() << " with length of " << msg->length() << std::endl;

    //echo it back
    (*c) << (*msg);

    delete msg;
}


void Server::onConnect(Connection* c)
{
    std::cout << "got connection from: " << c->address() << " : " << c->port() << std::endl;
}


void Server::onDisconnect(Connection* c)
{
    std::cout << "disconnection from: " << c->address() << " : " << c->port() << std::endl;

    
    // clean up the message box
    std::map<Connection*, Message*>::iterator itr = msgBox.find(c);

    if((itr != msgBox.end()) && (itr->second != NULL))
    {
        delete itr->second;
    }

    msgBox.erase(c);


    // remove from connections
    pool.remove(c);
    delete c;
}