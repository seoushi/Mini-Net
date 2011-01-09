/* 
 * File:   DataBuffer.hpp
 * Author: Sean Chapel
 *
 * Created on January 8, 2011, 1:41 PM
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
        void rewindPointer();

        // gets the size of the data buffer
        size_t size();

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
        long readLong();

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
        void writeShort(short s);

        // writes a char
        void writeChar(char c);

        // writes a int
        void writeInt(int i);

        // writes a long
        void writeLong(long l);

        // writes a string with a null terminator
        void writeString(std::string s);

        // writes a float
        void writeFloat(float f);

        // writes a double
        void writeDouble(double d);

    private:

        void* buffer;           // pointer to the data buffer
        size_t bufferSize;      // the ammount currently used in the buffer
        size_t maxBufferSize;   // the size of the allocated buffer
        size_t bufferPosition;  // the current position in the buffer
};

#endif	/* DATABUFFER_HPP */

