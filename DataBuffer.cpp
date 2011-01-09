/* 
 * File:   DataBuffer.cpp
 * Author: Sean Chapel
 * 
 * Created on January 8, 2011, 1:41 PM
 */

#include "DataBuffer.hpp"

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


void DataBuffer::rewindPointer()
{
    bufferPosition = 0;
}


size_t DataBuffer::size()
{
    return bufferSize;
}


void* DataBuffer::data()
{
    return buffer;
}


///
/// Read functions
///

// returns a pointer to the next element
void* DataBuffer::read(size_t size)
{
    if(!buffer || ((bufferPosition + size) > bufferSize))
    {
        void* data = (buffer + bufferPosition);
        bufferPosition += size;

        return data;
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


void DataBuffer::writeShort(short s)
{

}


void DataBuffer::writeChar(char c)
{

}


void DataBuffer::writeInt(int i)
{

}


void DataBuffer::writeLong(long l)
{

}


void DataBuffer::writeString(std::string s)
{

}


void DataBuffer::writeFloat(float f)
{

}


void DataBuffer::writeDouble(double d)
{

}
