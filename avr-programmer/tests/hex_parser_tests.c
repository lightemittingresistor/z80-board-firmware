/******************************************************************************
 * z80-board-firmware
 * hex_parser_tests.c - checks the hex parser works
 *
 *****************************************************************************/

#include "test_utils.h"
#include "hex-receiver.h"

#include <stdio.h>

TEST_INIT();

void feed_hex(hex_parser_context* ctx, const char* text, int length)
{
    for(int i = 0; i < length; ++i)
    {
        hex_parse_char(ctx, text[i]);
    }
}

bool check_rx_data(hex_parser_context* ctx, unsigned char* data, int length)
{
    if(ctx->byte_count != length)
    {
        return false;
    }

    for(int i = 0; i < length; ++i)
    {
        if(data[i] != ctx->data[i])
        {
            return false;
        }
    }

    return true;
}

void check_eof()
{
    const char eofstring[] = ":00000001FF";

    hex_parser_context ctx;
    hex_init(&ctx);
    feed_hex(&ctx, eofstring, sizeof(eofstring)-1);

    TEST_ASSERT(hex_parse_check_checksum(&ctx));
    TEST_ASSERT_EQ(ctx.record_type, HEX_RECORD_EOF);
    TEST_ASSERT_EQ(ctx.state, HEX_PARSER_STATE_DONE);
}

void check_data()
{
    const char datastring[] = ":1016700090959B01AC01BD01CF010895F894FFCF77";
    unsigned char data[] = 
        {   0x90, 0x95, 0x9B, 0x01, 
            0xAC, 0x01, 0xBD, 0x01,
            0xCF, 0x01, 0x08, 0x95,
            0xF8, 0x94, 0xFF, 0xCF };

    hex_parser_context ctx;
    hex_init(&ctx);
    feed_hex(&ctx, datastring, sizeof(datastring)-1);

    TEST_ASSERT(hex_parse_check_checksum(&ctx));
    TEST_ASSERT(check_rx_data(&ctx, data, sizeof(data)));
}

int main(int argc, char** argv)
{
    TEST_FUNCTION(check_eof);
    TEST_FUNCTION(check_data);

    TEST_END();
}