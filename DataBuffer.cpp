/* 
 * File:   DataBuffer.cpp
 * Author: Sean Chapel
 * 
 * Created on January 8, 2011, 1:41 PM
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
    // special case 0 as freeing memory
    if(newSize == 0)
    {
        free(buffer);
        buffer = NULL;
        bufferSize = 0;
        maxBufferSize = 0;
        bufferPosition = 0;
        
        return;
    }

    // if the buffer doesn't exist make it
    if(!buffer)
    {
        buffer = malloc(newSize);
    }
    else // resize the buffer
    {
        buffer = realloc(buffer, newSize);
    }

    // remeber how big it is
    maxBufferSize = newSize;

    // if the new size is smaller and the buffer position is past the end set it to the end
    if(bufferPosition >= newSize)
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


void* DataBuffer::data()
{
    return buffer + bufferPosition;
}


///
/// Read functions
///

// returns a pointer to the next element
void* DataBuffer::read(size_t size)
{
    if(!buffer || (size < spaceLeft()))
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


long DataBuffer::readLong()
{
    void* data = read(sizeof(long));

    if(data)
    {
        long* l = (long*)data;
        return *l;
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


void DataBuffer::write(long l)
{
    write((void*)&l, sizeof(long));
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
