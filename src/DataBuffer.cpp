/* 
 * File:   DataBuffer.cpp
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

#include <string.h>
#include <malloc.h>
#include <sstream>

DataBuffer::DataBuffer()
{
    buffer = NULL;
    bufferSize = 0;
    maxBufferSize = 0;
    bufferPosition = 0;
}


DataBuffer::~DataBuffer()
{
    free(buffer);
}



void DataBuffer::resize(size_t newSize)
{
    if(newSize == 0)
    {
        bufferSize = 0;
        bufferPosition = 0;
    }

    // remeber how big it is
    maxBufferSize = newSize;


    // if the buffer doesn't exist make it
    if(!buffer)
    {
        buffer = malloc(newSize);
    }
    else // resize the buffer
    {
        buffer = realloc(buffer, newSize);
    }

    

    // if the new size is smaller and the buffer position is past the end set it to the end
    if(bufferPosition == 0)
    {
         bufferPosition = 0;
    }
    else if(bufferPosition >= newSize)
    {
        bufferPosition = newSize - 1;
    }
}


void DataBuffer::rewind()
{
    bufferPosition = 0;
}


size_t DataBuffer::spaceLeft()
{
    return maxBufferSize - bufferPosition;
}


size_t DataBuffer::size()
{
    return bufferSize;
}


size_t DataBuffer::allocatedSize()
{
    return maxBufferSize;
}


void* DataBuffer::data()
{
    return (void*)(((char*)buffer) + bufferPosition);
}


///
/// Read functions
///

// returns a pointer to the next element
void* DataBuffer::read(size_t size)
{
    if(buffer && (size <= spaceLeft()))
    {
        void* dat = data();
        bufferPosition += size;

        return dat;
    }

    return NULL;
}


short DataBuffer::readShort()
{
    void* data = read(sizeof(short));

    if(data)
    {
        short* s = (short*)data;
        return *s;
    }

    return 0;
}


char DataBuffer::readChar()
{
    void* data = read(sizeof(char));

    if(data)
    {
        char* c = (char*)data;
        return *c;
    }

    return 0;
}


int DataBuffer::readInt()
{
    void* data = read(sizeof(int));

    if(data)
    {
        int* i = (int*)data;
        return *i;
    }

    return 0;
}


long long DataBuffer::readLongLong()
{
    void* data = read(sizeof(long long));

    if(data)
    {
        long long* l = (long long*)data;
        return *l;
    }

    return 0;
}


unsigned short DataBuffer::readUnsignedShort()
{
    void* data = read(sizeof(unsigned short));

    if(data)
    {
        unsigned short* us = (unsigned short*)data;
        return *us;
    }

    return 0;
}


unsigned char DataBuffer::readUnsignedChar()
{
    void* data = read(sizeof(unsigned char));

    if(data)
    {
        unsigned char* uc = (unsigned char*)data;
        return *uc;
    }

    return 0;
}


unsigned int DataBuffer::readUnsignedInt()
{
    void* data = read(sizeof(unsigned int));

    if(data)
    {
        unsigned int* ui = (unsigned int*)data;
        return *ui;
    }

    return 0;
}


unsigned long long DataBuffer::readUnsignedLongLong()
{
    void* data = read(sizeof(unsigned long long));

    if(data)
    {
        unsigned long long* ul = (unsigned long long*)data;
        return *ul;
    }

    return 0;
}


float DataBuffer::readFloat()
{
    void* data = read(sizeof(float));

    if(data)
    {
        float* f = (float*)data;
        return *f;
    }

    return 0.0f;
}


double DataBuffer::readDouble()
{
    void* data = read(sizeof(double));

    if(data)
    {
        double* d = (double*)data;
        return *d;
    }

    return 0.0f;
}


std::string DataBuffer::readString()
{
    std::stringstream ss;
    char c;

    for(;;)
    {
        c = readChar();

        if(c == 0)
        {
            break;
        }
        else
        {
            ss << c;
        }
    }

    return ss.str();
}


///
/// write functions
///


void DataBuffer::write(void* dataP, size_t length)
{
    // no buffer? buffer not big enough? resize it
    if(spaceLeft() < length)
    {
        resize(maxBufferSize - spaceLeft() + length);
    }

    memcpy(data(), dataP, length);

    bufferPosition += length;
    bufferSize += length;
}

void DataBuffer::write(short s)
{
    write((void*)&s, sizeof(short));
}


void DataBuffer::write(char c)
{
    write((void*)&c, sizeof(char));
}


void DataBuffer::write(int i)
{
    write((void*)&i, sizeof(int));
}


void DataBuffer::write(long long l)
{
    write((void*)&l, sizeof(long long));
}


void DataBuffer::write(unsigned short us)
{
    write((void*)&us, sizeof(unsigned short));
}


void DataBuffer::write(unsigned char uc)
{
    write((void*)&uc, sizeof(unsigned char));
}


void DataBuffer::write(unsigned int ui)
{
    write((void*)&ui, sizeof(unsigned int));
}


void DataBuffer::write(unsigned long long ul)
{
    write((void*)&ul, sizeof(unsigned long long));
}


void DataBuffer::write(std::string s)
{
    write((void*)&s[0], s.length() * sizeof(char));
    write((char)0);
}


void DataBuffer::write(float f)
{
    write((void*)&f, sizeof(float));
}


void DataBuffer::write(double d)
{
    write((void*)&d, sizeof(double));
}
