/******************************************************************************
 * z80-board-firmware
 * protocol.c - Communications protocol for upload/download
 * In multiple files for testing purposes
 *****************************************************************************/

#include "protocol/protocol.h"
#include "protocol_internal.h"

void protocol_init()
{
    protocol_assm_init();
}

uint8_t protocol_checksum(unsigned char* bytes, unsigned int length)
{
    uint8_t sum = 0;
    for(unsigned int i = 0; i < length; ++i)
    {
        sum += bytes[i];
    }
    return (~sum)-1;
}