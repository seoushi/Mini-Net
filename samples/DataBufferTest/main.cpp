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

#include "DataBuffer.hpp"

#include <iostream>
#include <sstream>
#include <string.h>

// data buffer values
int     i = -1;
short   s = -20;
char    c = 'c';
float   f = -1.0f;
double  d = 1.0f;
unsigned int       ui = -1;
unsigned short     us = -20;
unsigned char      uc = 200;
long long          ll = -234563454;
unsigned long long ull = 234563454;
const char*        str = "testing a string";


int testServer();
int testDataBuffer();

bool doWriteTest(DataBuffer* buffer)
{
    size_t bufferSize = sizeof(i) + sizeof(s) + sizeof(c) + sizeof(f) +
            sizeof(d) + sizeof(ui) + sizeof(us) + sizeof(uc) + sizeof(ll)
            + sizeof(ull) + (sizeof(char) * (strlen(str) + 1));

    std::cout << "Writing to buffer: ";
    (*buffer) << i << s << c << f << d << ui << us << uc << ll << ull
            << std::string(str);
    
    if(buffer->size() == bufferSize)
    {
        std::cout << "[PASSED]" << std::endl;
        return true;
    }

    std::cout << "[FAILED] buffer size is'" << buffer->size() <<
            "' should have been '" << bufferSize << "'" << std::endl;

    return false;
}


bool doReadTest(DataBuffer* buffer)
{
    int ri;
    short rs;
    char rc;
    float rf;
    double rd;
    unsigned int rui;
    unsigned short rus;
    unsigned char ruc;
    long long rll;
    unsigned long long rull;
    std::string rstr;

    bool error = false;
    std::stringstream errorStream;

    // do reading
    std::cout << "Reading from buffer: ";

    (*buffer) >> ri >> rs >> rc >> rf >> rd >> rui >> rus >> ruc >> rll >> rull >> rstr;


    // check values
    if(c != rc)
    {
        error = true;
        errorStream << "\t char '" << c << "' != '" << rc << "'" << std::endl;
    }

    if(i != ri)
    {
        error = true;
        errorStream << "\t int '" << i << "' != '" << ri << "'" << std::endl;
    }

    if(ll != rll)
    {
        error = true;
        errorStream << "\t long long '" << ll << "' != '" << rll << "'" << std::endl;
    }

    if(f != rf)
    {
        error = true;
        errorStream << "\t float '" << f << "' != '" << rf << "'" << std::endl;
    }

    if(d != rd)
    {
        error = true;
        errorStream << "\t double '" << d << "' != '" << rd << "'" << std::endl;
    }

    if(s != rs)
    {
        error = true;
        errorStream << "\t short'" << s << "' != '" << rs << "'" << std::endl;
    }

    if(uc != ruc)
    {
        error = true;
        errorStream << "\t unsigned char '" << uc << "' != '" << ruc << "'" << std::endl;
    }

    if(ui != rui)
    {
        error = true;
        errorStream << "\t unsigned int '" << ui << "' != '" << rui << "'" << std::endl;
    }

    if(ull != rull)
    {
        error = true;
        errorStream << "\t unsigned long '" << ull << "' != '" << rull << "'" << std::endl;
    }

    if(us != rus)
    {
        error = true;
        errorStream << "\t unsigned short '" << us << "' != '" << rus << "'" << std::endl;
    }

    if(str != rstr)
    {
        error = true;
        errorStream << "\t std::string '" << str << "' != '" << rstr << "'" << std::endl;
    }

    if(!error)
    {
        std::cout << "[PASSED]" << std::endl;
        return true;
    }

    std::cout << "[FAILED] : " << std::endl << errorStream.str() << std::endl;

    return false;
}


int main(int argc, char** argv)
{
    int numTests = 3;
    int testsPassed = 0;

    DataBuffer buffer;

    if(doWriteTest(&buffer))
    {
        testsPassed++;
    }
    
    buffer.rewind();

    if(doReadTest(&buffer))
    {
        testsPassed++;
    }

    // test resizing too small, failing is crashing :(
    buffer.resize(0);
    std::cout << "Resizing: [PASSED]" << std::endl << std::endl;
    testsPassed++;

    std::cout << "Passed " << testsPassed << " test and failed " << numTests - testsPassed << std::endl;


    return 0;
}