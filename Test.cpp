/* 
 * File:   main.cpp
 * Author: SChapel
 *
 * Created on January 5, 2011, 8:47 AM
 */

#include "Connection.hpp"
#include "ConnectionPool.hpp"
#include "DataBuffer.hpp"

#include <iostream>

int testServer();
int testDataBuffer();

int main(int argc, char** argv)
{
    return testDataBuffer();
}

int testServer()
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


int testDataBuffer()
{
    int numTests = 2;
    int testsPassed = 0;

    DataBuffer buffer;


    // Do write/size testing
    std::cout << "writing: char 'a'" << std::endl;
    buffer.write('a');
    
    std::cout << "writing: int '2'" << std::endl;
    buffer.write((int)2);

    std::cout << "writing: long '3'" << std::endl;
    buffer.write((long)3);

    std::cout << "writing: float '4.0f'" << std::endl;
    buffer.write((float)4.0f);

    std::cout << "writing: double '5.0f'" << std::endl;
    buffer.write((double)5.0f);

    std::cout << "writing: short '6'" << std::endl;
    buffer.write((short)6);
    
    std::cout << "writing: string 'testing a string'" << std::endl;
    buffer.write(std::string("testing a string"));
    
    
    std::cout << "buffer size should be: 44 " << std::endl;
    std::cout << "buffer size is: " << buffer.size() << std::endl;
    if(buffer.size() == 44)
    {
        std::cout << "size test: [PASSED]" << std::endl;
        testsPassed++;
    }
    else
    {
        std::cout << "size test: [FAILED]" << std::endl;
    }
    
    std::cout << std::endl << std::endl;



    // do reading test
    buffer.rewind();

    char c = buffer.readChar();
    std::cout << "read char: 'a' == '" << c << "'" << std::endl;

    int i = buffer.readInt();
    std::cout << "read int: 2 == " << i << std::endl;

    long l = buffer.readLong();
    std::cout << "read long: 3 == " << l << std::endl;

    float f = buffer.readFloat();
    std::cout << "read float: 4.0f == " << f << std::endl;

    double d = buffer.readDouble();
    std::cout << "read double: 5.0f == " << d << std::endl;

    short s = buffer.readShort();
    std::cout << "read short: 6 == " << s << std::endl;

    std::string str = buffer.readString();
    std::cout << "read string: 'testing a string' == '" << str << "'" << std::endl;

    if((c == 'a') &&
       (i == 2) &&
       (l == 3) &&
       (f == 4.0f) &&
       (d == 5.0f) &&
       (s == 6) &&
       (str == std::string("testing a string")))
    {
        std::cout << "content test: [PASSED]" << std::endl;
        testsPassed++;
    }
    else
    {
        std::cout << "content test: [FAILED]" << std::endl;
    }


    std::cout << std::endl << std::endl;

    std::cout << "Passed " << testsPassed << " test and failed " << numTests - testsPassed << std::endl;


    return 0;
}