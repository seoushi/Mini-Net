/*
 * File:   Message.cpp
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

#include "Message.hpp"


Message::Message()
{
    length = 0;
    hasReadLength = false;
    bytesLeft = sizeof(unsigned short);
    buffer.resize(bytesLeft);
}


Message::~Message()
{
}


int Message::read(Connection* con)
{
    int bytesRead = 0;

    // don't read if we don't need to
    if(!bytesLeft)
    {
        return 0;
    }

    // find the read offset
    int offset = length - bytesLeft;

    if(offset < 0)
    {
        offset = 0;
    }


    // try to read as much as we need from the connection
    bytesRead = con->read((char*)buffer.data() + offset, bytesLeft);
    bytesLeft -= bytesRead;


    // we haven't read the length yet and are done reading the length
    if(!hasReadLength && !bytesLeft)
    {
        buffer >> length;
        bytesLeft = length;

        buffer.resize(length);
        buffer.rewind();

        hasReadLength = true;
    }

    if(!bytesLeft)
    {
        buffer.rewind();
    }

    return bytesRead;
}


void Message::write(Connection* con)
{
    con->write((char*)&length, sizeof(length));
    con->write((char*)buffer.data(), buffer.size());
}

 
bool Message::isComplete()
{
    // if the length hasn't been read or the buffer has some reading to do
    if(!hasReadLength || bytesLeft)
    {
        return false;
    }

    return true;
}


void Message::setData(DataBuffer buffer)
{
    buffer.rewind();
    this->buffer = buffer;

    length = buffer.size();
}


DataBuffer* Message::getData()
{
    return &buffer;
}


size_t Message::getLength()
{
    return (size_t)length;
}
