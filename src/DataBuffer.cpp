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

DataBuffer& DataBuffer::operator >>(short& val)
{
    void* data = read(sizeof(val));
    val = data ? *((short*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(char& val)
{
    void* data = read(sizeof(val));
    val = data ? *((char*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(int& val)
{
    void* data = read(sizeof(val));
    val = data ? *((int*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(long long& val)
{
    void* data = read(sizeof(val));
    val = data ? *((long long*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned short& val)
{
    void* data = read(sizeof(val));
    val = data ? *((unsigned short*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned char& val)
{
    void* data = read(sizeof(val));
    val = data ? *((unsigned char*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned int& val)
{
    void* data = read(sizeof(val));
    val = data ? *((unsigned int*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(unsigned long long& val)
{
    void* data = read(sizeof(val));
    val = data ? *((unsigned long long*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(float& val)
{
    void* data = read(sizeof(val));
    val = data ? *((float*)data) : 0;
    return *this;
}


DataBuffer& DataBuffer::operator >>(double& val)
{
    void* data = read(sizeof(val));
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


DataBuffer& DataBuffer::operator <<(const short& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const char& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const int& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const long long& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned short& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned char& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned int& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const unsigned long long& val)
{
    write((void*)&val, sizeof(val));
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
    write((void*)&val, sizeof(val));
    return *this;
}


DataBuffer& DataBuffer::operator <<(const double& val)
{
    write((void*)&val, sizeof(val));
    return *this;
}
