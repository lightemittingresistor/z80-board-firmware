/******************************************************************************
 * z80-board-firmware
 * hex-receiver.h - code to receive Intel Hex files
 *
 *****************************************************************************/

#ifndef Z80_BOARD_HEX_RECEIVER_H
#define Z80_BOARD_HEX_RECEIVER_H

#include <stdbool.h>

enum 
{
    HEX_PARSER_STATE_INIT,
    HEX_PARSER_STATE_WAIT_HEADER,
    HEX_PARSER_STATE_WAIT_DATA,
    HEX_PARSER_STATE_CHECKSUM,
    HEX_PARSER_STATE_GOT_DATA,
    HEX_PARSER_STATE_DONE,
};

enum
{
    HEX_RECORD_DATA = 0,
    HEX_RECORD_EOF = 1,
};

typedef struct 
{
    unsigned char data[256];
    int state;
    int section_digit;

    short byte_count;
    unsigned int address;
    short record_type;
    unsigned char checksum;
    unsigned char calculated_checksum;
    int checksum_digit;
    unsigned char checksum_scratch;

    unsigned int rxcount;
} hex_parser_context;

void hex_init(hex_parser_context* ctx);
void hex_parse_char(hex_parser_context* ctx, char c);
bool hex_parse_check_checksum(hex_parser_context* ctx);


#endif
