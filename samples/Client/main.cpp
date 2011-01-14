/* 
 * File:   main.cpp
 * Author: sean
 *
 * Created on January 13, 2011, 7:29 PM
 */

#include <iostream>

#include "Connection.hpp"
#include "Message.hpp"


int main(int argc, char** argv)
{
    Connection con;
    if(!con.connect("127.0.0.1", 7777))
    {
        std::cout << "could not connect to server, exiting" << std::endl;
        
        return 1;
    }


    std::string input;

    while(input != "quit!")
    {

        // get user message
        std::cin >> input;

        // send user message
        DataBuffer buffer;
        Message outMsg;

        buffer.write(input);
        outMsg.setData(buffer);
        outMsg.write(&con);

        // get server response
        Message inMsg;
        
        while(!inMsg.isComplete())
        {
            inMsg.read(&con);
        }

        std::cout << "got reply: " << inMsg.getData()->readString() << std::endl;
    }
        


    return 0;
}

