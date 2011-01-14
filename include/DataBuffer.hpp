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

/**
 * The Data Buffer class holds a chunk of memory to read and write to. Data
 * Buffers will automatically resize when written to when needed.
 */
class DataBuffer
{
    public:

        /**
         * Default constructor
         */
        DataBuffer();

        /**
         * Default destructor
         */
        ~DataBuffer();

        /**
         * Resizes the data buffer to a given size.
         * @param newSize the new size for the buffer
         */
        void resize(size_t newSize);

        /**
         * Sets the current data pointer to the begining of the buffer
         */
        void rewind();

        /**
         * Gets the size of the data buffer currently in use
         * @return the buffers size
         */
        size_t size();

        /**
         * Gets the max size the data buffer can hold
         * @return the buffer's max size
         */
        size_t allocatedSize();

        /**
         * Gets the space left for writing in the buffer
         * @return the space left in the buffer
         */
        size_t spaceLeft();

        /**
         * Gets a pointer to the current position in the buffer
         * @return pointer to the current position in the buffer
         */
        void* data();


    /***************************************************************************
     * READ FUNCTIONS
     *
     * All read functions move the data pointer forward by the amount read
     **************************************************************************/


        /**
         * Gets a chunk of memory from the buffer
         * @param size the amount to read
         * @return the data read
         */
        void* read(size_t size);

        /**
         * Reads a short from the buffer
         * @return a short
         */
        short readShort();

        /**
         * Reads a char from the buffer
         * @return a char
         */
        char readChar();

        /**
         * Reads an int from the buffer
         * @return an int
         */
        int readInt();

        /**
         * Reads a long long from the buffer
         * @return a long long
         */
        long long readLongLong();

        /**
         * Reads a unsigned short from the buffer
         * @return a unsigned short
         */
        unsigned short readUnsignedShort();

        /**
         * Reads a unsigned char from the buffer
         * @return a unsigned char
         */
        unsigned char readUnsignedChar();

        /**
         * Reads a unsigned int from the buffer
         * @return a unsigned int
         */
        unsigned int readUnsignedInt();

        /**
         * Reads a unsigned long long from the buffer
         * @return a unsigned long long
         */
        unsigned long long readUnsignedLongLong();

        /**
         * Reads a float from the buffer
         * @return a float
         */
        float readFloat();

        /**
         * Reads a double from the buffer
         * @return a double
         */
        double readDouble();

        /**
         * Reads a null terminated string from the buffer
         * @return a std::string
         */
        std::string readString();



    /***************************************************************************
     * WRITE FUNCTIONS
     *
     * All write functions will increase the size of the buffer as needed
     **************************************************************************/


        /**
         * Writes a chunk of memory to the buffer
         * @param data the data to write to the buffer
         * @param length the length of the data to write
         */
        void write(void* data, size_t length);


        /**
         * Writes a short to the buffer
         * @param s the short to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const short& s);

        /**
         * Writes a char to the buffer
         * @param c the char to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const char& c);

        /**
         * Writes an int to the buffer
         * @param i the int to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const int& i);

        /**
         * Writes a long long to the buffer
         * @param ll the long long to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const long long& ll);

        /**
         * Writes a unsigned short to the buffer
         * @param us the unsigned short to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const unsigned short& us);

        /**
         * Writes a unsigned char to the buffer
         * @param uc the unsigned char to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const unsigned char& uc);

        /**
         * Writes a unsigned int to the buffer
         * @param ui the unsigned int to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const unsigned int& ui);

        /**
         * Writes a unsigned long long to the buffer
         * @param ull the unsigned long long to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const unsigned long long& ull);

        /**
         * Writes a std::string to the buffer
         * @param s the string to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const std::string& s);

        /**
         * Writes a float to the buffer
         * @param f the float to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const float& f);

        /**
         * Writes a double to the buffer
         * @param d the double to write
         * @return a reference to the DataBuffer
         */
        DataBuffer& operator <<(const double& d);

    private:

        void* buffer;           /**< pointer to the data buffer               */
        size_t bufferSize;      /**< the amount currently used in the buffer  */
        size_t maxBufferSize;   /**< the size of the allocated buffer         */
        size_t bufferPosition;  /**< the current position in the buffer       */
};

#endif	/* DATABUFFER_HPP */

