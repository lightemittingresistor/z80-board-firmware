/******************************************************************************
 * z80-board-firmware
 * protocol.h - Communications protocol for upload/download
 *
 *****************************************************************************/

#ifndef Z80_BOARD_DEVICE_PROTOCOL_H
#define Z80_BOARD_DEVICE_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

#define PROTOCOL_START_CODE 0x5A

/*****************************************************************************
 * Overall Message Structure 
 * ==========================
 *
 * byte: contents
 * --------------
 * 0: 0x5A
 * 1: length (including checksum)
 * 2: message type code
 * 3...n-1: payload
 * n: checksum
 * --------------
 *
 * Serial adds ACKs and NAKs on each message
 *
 *****************************************************************************/

// The implementation of these is split across multiple files
// to make it easier to test

void protocol_init();

// Take an arbitrary number of bytes and parse the message or partial message
void protocol_feed_bytes(unsigned char* bytes, unsigned int length);

// Take exactly one message and parse it.
void protocol_feed_message(unsigned char* bytes, unsigned int length);
    
// Calculate message checksum
uint8_t protocol_checksum(unsigned char* bytes, unsigned int length);

// Send a message. Returns true on success, false on failure
bool protocol_send_data(unsigned char* bytes, unsigned int length);

// send an ACK or NAK if the wrapper protocol requires it (aka if serial)
void protocol_send_ack();
void protocol_send_nak();

#endif
