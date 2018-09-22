/******************************************************************************
 * z80-board-firmware
 * protocol_assembler.c - Communications protocol for upload/download
 * In multiple files for testing purposes
 *****************************************************************************/

#include "protocol/protocol.h"
#include "protocol_internal.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define PROTOCOL_ASSM_SCRATCH_SPACE_SIZE 256

struct 
{
    uint8_t buffer[PROTOCOL_ASSM_SCRATCH_SPACE_SIZE];
    uint8_t cursor;
    bool haveIncomingMessage;
} protocol_assm_working;

void protocol_assm_init()
{
    protocol_assm_working.haveIncomingMessage = false;
    protocol_assm_working.cursor = 0;
}

void protocol_feed_bytes(unsigned char* bytes, unsigned int length)
{
    while(length)
    {
        if(!protocol_assm_working.haveIncomingMessage)
        {
            // look for start code
            while(length && bytes[0] != 'Z')
            {
                --length;
                ++bytes;
            }
            if(!length)
            {
                return;
            }
            // found a start
            protocol_assm_working.haveIncomingMessage = true;
        }

        unsigned int currentLength = length;
        if((protocol_assm_working.cursor+length) > PROTOCOL_ASSM_SCRATCH_SPACE_SIZE)
        {
            currentLength = PROTOCOL_ASSM_SCRATCH_SPACE_SIZE;
        }

        //find the message length. We might need to look in a few places!
        unsigned int messageLength = 0;
        if(protocol_assm_working.cursor == 0)
        {
            // if we have enough in this buffer, use it
            if(length > 1)
            {
                messageLength = bytes[1];
            }
            else
            {
                // otherwise we just take the first byte this trip round
                messageLength = 1;
            }
        }
        else if(protocol_assm_working.cursor == 1)
        {
            // the length is at the beginning of this buffer
            // Add 2 for start code and length
            messageLength = bytes[0]+2;
        }
        else
        {
            // message length is in the scratch space
            messageLength = protocol_assm_working.buffer[1]+2;
        }   

        // figure out how much we need to get a message
        if(currentLength > (messageLength - protocol_assm_working.cursor))
        {
            currentLength = (messageLength - protocol_assm_working.cursor);
        }
        memcpy(protocol_assm_working.buffer+protocol_assm_working.cursor, bytes, currentLength);
        protocol_assm_working.cursor += currentLength;

        if(protocol_assm_working.cursor > 1)
        {
            uint8_t msgLength = protocol_assm_working.buffer[1];
            if((protocol_assm_working.cursor-2) == msgLength)
            {
                protocol_feed_message(protocol_assm_working.buffer, protocol_assm_working.cursor);
                protocol_assm_working.haveIncomingMessage = false;
                protocol_assm_working.cursor = 0;
            }
        }
        bytes += currentLength;
        length -= currentLength;
    }
}
