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

        if(!con.isConnected())
        {
            std::cout << "disconnected from server: exiting" << std::endl;
        }

        // send user message
        DataBuffer buffer;
        Message outMsg;

        buffer << input;
        outMsg.setData(buffer);
        con << outMsg;

        // get server response
        Message inMsg;
        
        for(;;)
        {
            if(con >> inMsg)
            {
                break;
            }
        }

        std::string reply;
        inMsg.data() >> reply;
        std::cout << "got reply: " << reply << std::endl;
    }
        


    return 0;
}

