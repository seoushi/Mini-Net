/*
 * File:   Message.hpp
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

#ifndef MESSAGE_H
#define	MESSAGE_H

/**
 * The Message class bridges information from the DataBuffer and Connection
 * classes. The Message class first reads the length of the message and then
 * populates the data. When the Message is populated to it's length it doesn't
 * read any more and is marked as complete.
 */
class Message
{
    public:

        /**
         * Default constructor
         */
        Message();

        /**
         * Default destructor
         */
        ~Message();

        /**
         * Reads data from a connection
         * @param con the connection to read data from
         * @return the number of bytes read
         */
        int read(Connection* con);

        /**
         * Writes data to a connection
         * @param con the connection to write data to
         */
        void write(Connection* con);

        /**
         * Tells if the message has read all of the data needed
         * @return true if the message message has its data otherwise false
         */
        bool isComplete();

        /**
         * Sets the message's data to the new buffer
         * @param buffer the buffer to set the data too
         */
        void setData(DataBuffer buffer);


        /**
         * Gets the data from the buffer
         * @return the buffer's data
         */
        DataBuffer* getData();

        /**
         * Gets the length of the message
         * @return the message length
         */
        size_t getLength();

    private:

        bool hasReadLength;         /**< If the msg length has been determined*/
        unsigned short bytesLeft;   /**< The bytes left to read               */
        unsigned short length;      /**< The length of the message to read    */
        DataBuffer buffer;          /**< The databuffer to store the data     */
};

#endif	/* MESSAGE_H */

