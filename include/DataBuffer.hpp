/* 
 * File:   DataBuffer.hpp
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

#ifndef DATABUFFER_HPP
#define	DATABUFFER_HPP

#include <stddef.h>
#include <string>

class DataBuffer
{
    public:

        // default constructor
        DataBuffer();

        // default destructor
        ~DataBuffer();

        // resizes the data buffer to a given size
        // if you resize too small data will be lost
        void resize(size_t newSize);

        // sets the current data pointer to the begining
        void rewind();

        // gets the size of the data buffer currently in use
        size_t size();

        // gets the max size the data buffer can hold
        size_t allocatedSize();

        // returns the ammount of space avialable for writting
        size_t spaceLeft();

        // gets a pointer to the current position in the buffer
        void* data();


        ///
        /// read functions. All read functions move the 
        /// data pointer forward by the ammount read
        ///

        // returns a pointer to the next element
        void* read(size_t size);

        // reads a short
        short readShort();

        // reads a char
        char readChar();

        // reads a int
        int readInt();

        // reads a long
        long long readLongLong();

        // reads a unsigned short
        unsigned short readUnsignedShort();

        // reads a unsigned char
        unsigned char readUnsignedChar();

        // reads a unsigned int
        unsigned int readUnsignedInt();

        // reads a unsigned long
        unsigned long long readUnsignedLongLong();

        // reads a float
        float readFloat();

        //reads a double
        double readDouble();

        // reads a string (till a null terminator)
        std::string readString();


        ///
        /// write functions. write functions will all increase the
        /// size of the buffer as needed
        ///

        // writes a chunk of memory to the buffer
        void write(void* data, size_t length);

        // writes a short
        void write(short s);

        // writes a char
        void write(char c);

        // writes a int
        void write(int i);

        // writes a long
        void write(long long l);

        // writes a unsigned short
        void write(unsigned short us);

        // writes a unsigned char
        void write(unsigned char uc);

        // writes a unsigned int
        void write(unsigned int ui);

        // writes a unsigned long
        void write(unsigned long long ul);

        // writes a string with a null terminator
        void write(std::string s);

        // writes a float
        void write(float f);

        // writes a double
        void write(double d);

    private:

        void* buffer;           // pointer to the data buffer
        size_t bufferSize;      // the ammount currently used in the buffer
        size_t maxBufferSize;   // the size of the allocated buffer
        size_t bufferPosition;  // the current position in the buffer
};

#endif	/* DATABUFFER_HPP */

