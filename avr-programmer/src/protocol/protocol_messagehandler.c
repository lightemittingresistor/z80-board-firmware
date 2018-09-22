/******************************************************************************
 * z80-board-firmware
 * protocol_messagehandler.c - Communications protocol for upload/download
 * In multiple files for testing purposes
 *****************************************************************************/

#include "protocol/protocol.h"
#include "protocol/messagetypes.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <avr/pgmspace.h>

void protocol_feed_message(unsigned char* bytes, unsigned int length)
{
    if(length == 0)
    {
        // Nothing to do!
        return;
    }

    // should be zero
    if(protocol_checksum(bytes, length))
    {
        protocol_send_nak();
        return;
    }
    else
    {
        protocol_send_ack();
    }

    switch(bytes[2])
    {
    case msg_type_ping:
        {
            msg_ping_t ping;
            msg_ping_deserialise(&ping, bytes, length);
            msg_pong_t pong;
            pong.serial = ping.serial;
            uint8_t buffer[16];
            msg_pong_serialise(&pong, buffer, 16);
            protocol_send_data(buffer, buffer[1]+2);
        } break;
    }
}

void msg_ping_serialise(const msg_ping_t* msg, uint8_t* buffer, uint8_t length)
{
    if(length < 5) // 4 bytes padding plus 1 byte payload
    {
        return;
    }

    buffer[0] = PROTOCOL_START_CODE;
    buffer[1] = 3;
    buffer[2] = msg_type_ping;
    buffer[3] = msg->serial;
    buffer[4] = protocol_checksum(buffer, 3);
}

void msg_ping_deserialise(msg_ping_t* msg, const uint8_t* buffer, uint8_t length)
{
    if(buffer[1] != 3)
    {
        // invalid message length
        return;
    }
    if(buffer[2] != msg_type_ping)
    {
        // wrong message type
        return;
    }

    msg->serial = buffer[3];
}

void msg_pong_serialise(const msg_pong_t* msg, uint8_t* buffer, uint8_t length)
{
    if(length < 5) // 4 bytes padding plus 1 byte payload
    {
        return;
    }

    buffer[0] = PROTOCOL_START_CODE;
    buffer[1] = 3;
    buffer[2] = msg_type_pong;
    buffer[3] = msg->serial;
    buffer[4] = protocol_checksum(buffer, 3);
}

void msg_pong_deserialise(msg_pong_t* msg, const uint8_t* buffer, uint8_t length)
{
    if(buffer[1] != 3)
    {
        // invalid message length
        return;
    }
    if(buffer[2] != msg_type_pong)
    {
        // wrong message type
        return;
    }

    msg->serial = buffer[3];
}

void msg_debug_serialise(const msg_debug_t* msg, uint8_t* buffer, uint8_t length)
{
    if(length < msg->length+4) // 4 bytes padding plus n byte payload
    {
        return;
    }

    buffer[0] = PROTOCOL_START_CODE;
    buffer[1] = 1 + msg->length;
    buffer[2] = msg_type_debug;
    if(msg->in_pgm_space)
    {
        memcpy_P(buffer+3, msg->dbg_string, msg->length);
    }
    else 
    {
        memcpy(buffer+3, msg->dbg_string, msg->length);
    }
    buffer[msg->length+3] = protocol_checksum(buffer, msg->length+3);
}

void msg_debug_deserialise(msg_debug_t* msg, const uint8_t* buffer, uint8_t length)
{
    if(buffer[2] != msg_type_debug)
    {
        // wrong message type
        return;
    }

    msg->length = buffer[1]-2;
    msg->in_pgm_space = false;
    msg->dbg_string = (const char*)(buffer+2);
}

