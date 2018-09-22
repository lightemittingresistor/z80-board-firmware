/******************************************************************************
 * z80-board-firmware
 * messagetypes.h - Communications protocol definitions
 *
 *****************************************************************************/

#ifndef Z80_BOARD_DEVICE_PROTOCOL_MESSAGETYPES_H
#define Z80_BOARD_DEVICE_PROTOCOL_MESSAGETYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum 
{
    msg_type_ping = 1,
    msg_type_pong = 2,
    msg_type_debug = 3,
    msg_type_info = 4,
    msg_type_writedata = 5,
    msg_type_readdata = 6,
    msg_type_writeio = 7,
    msg_type_readio = 8,
    msg_type_datablob = 9,
} msg_type_t;

typedef struct
{
    uint8_t serial;
} msg_ping_t;

typedef struct
{
    uint8_t serial;
} msg_pong_t;

typedef struct 
{
    uint8_t length;
    bool in_pgm_space; // not part of the wire protocol
    const char* dbg_string;
} msg_debug_t;


void msg_ping_serialise(const msg_ping_t* msg, uint8_t* buffer, uint8_t length);
void msg_ping_deserialise(msg_ping_t* msg, const uint8_t* buffer, uint8_t length);

void msg_pong_serialise(const msg_pong_t* msg, uint8_t* buffer, uint8_t length);
void msg_pong_deserialise(msg_pong_t* msg, const uint8_t* buffer, uint8_t length);

void msg_debug_serialise(const msg_debug_t* msg, uint8_t* buffer, uint8_t length);
// Note: the supplied msg object references the decode buffer!
void msg_debug_deserialise(msg_debug_t* msg, const uint8_t* buffer, uint8_t length);

#endif
