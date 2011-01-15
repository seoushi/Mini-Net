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
#include "Connection.hpp"

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

DataBuffer::DataBuffer(char* data, size_t size)
{
    buffer = NULL;
    bufferPosition = 0;
    bufferSize = 0;
    maxBufferSize = 0;

    write(data, size);
}


DataBuffer::DataBuffer(DataBuffer& buf)
{
    buffer = NULL;
    bufferPosition = 0;
    bufferSize = 0;
    maxBufferSize = 0;

    write(buf.data(), buf.size());
}


DataBuffer::~DataBuffer()
{
    free(buffer);
}


void DataBuffer::clear()
{
    resize(0);
}


size_t DataBuffer::position()
{
    return bufferPosition;
}


void DataBuffer::resize(size_t newSize)
{
    if(newSize == 0)
    {
        bufferSize = 0;
        bufferPosition = 0;
    }

    // if the buffer doesn't exist make it
    if(!buffer)
    {
        buffer = (char*)malloc(newSize);
    }
    else // resize the buffer
    {
        buffer = (char*)realloc(buffer, newSize);
    }

    // if the new size is smaller
    if(newSize < maxBufferSize)
    {
        bufferPosition = 0;
    }


    // remeber how big it is
    maxBufferSize = newSize;
}


void DataBuffer::rewind()
{
    bufferPosition = 0;
}


void DataBuffer::seek(size_t bytes)
{
    bufferPosition += bytes;

    if(bufferPosition < 0)
    {
        bufferPosition = 0;
    }
    if(bufferPosition >= maxBufferSize)
    {
        bufferPosition = maxBufferSize;
    }
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


char* DataBuffer::data()
{
    return buffer;
}


///
/// Read functions
///

// returns a pointer to the next element
char* DataBuffer::read(size_t size)
{
    if(buffer && (size <= spaceLeft()))
    {
        char* dat = (buffer + bufferPosition);
        seek(size);

        return dat;
    }

    return NULL;
}

DataBuffer& DataBuffer::operator >>(short& val)
{
    char* data = read(sizeof(val));
    val = data ? *((short*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(char& val)
{
    char* data = read(sizeof(val));
    val = data ? *((char*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(int& val)
{
    char* data = read(sizeof(val));
    val = data ? *((int*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(long long& val)
{
    char* data = read(sizeof(val));
    val = data ? *((long long*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned short& val)
{
    char* data = read(sizeof(val));
    val = data ? *((unsigned short*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned char& val)
{
    char* data = read(sizeof(val));
    val = data ? *((unsigned char*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned int& val)
{
    char* data = read(sizeof(val));
    val = data ? *((unsigned int*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned long long& val)
{
    char* data = read(sizeof(val));
    val = data ? *((unsigned long long*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(float& val)
{
    char* data = read(sizeof(val));
    val = data ? *((float*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(double& val)
{
    char* data = read(sizeof(val));
    val = data ? *((double*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(std::string& val)
{
    std::stringstream ss;
    char c;

    for(;;)
    {
        *this >> c;

        if(c == 0)
        {
            break;
        }
        else
        {
            ss << c;
        }
    }

    val = ss.str();
    return *this;
}


///
/// write functions
///


void DataBuffer::write(char* dataP, size_t length)
{
    // no buffer? buffer not big enough? resize it
    if(spaceLeft() < length)
    {
        resize(maxBufferSize - spaceLeft() + length);
    }

    memcpy(buffer + bufferPosition, dataP, length);
    bufferSize += length;

    seek(length);
}


DataBuffer& DataBuffer::operator <<(const short& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const char& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const int& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const long long& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned short& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned char& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned int& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned long long& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const std::string& val)
{
    for(std::string::const_iterator itr = val.begin(); itr != val.end(); itr++)
    {
        *this << (*itr);
    }
    *this << (char)0;
    
    return *this;
}


DataBuffer& DataBuffer::operator <<(const float& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const double& val)
{
    write((char*)&val, sizeof(val));
    return *this;
}
