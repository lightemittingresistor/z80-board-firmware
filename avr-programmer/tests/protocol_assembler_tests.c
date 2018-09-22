/******************************************************************************
 * z80-board-firmware
 * protocol_assembler_tests.c - checks the message assembler works
 *
 *****************************************************************************/

#include "protocol/protocol.h"

#include "test_utils.h"

#include <string.h>
#include <stdint.h>

// scratch space for incoming messages
unsigned char parsed_buffer[256];
unsigned int parsed_length = 0;
unsigned int message_count = 0;
    
void protocol_feed_message(uint8_t* bytes, unsigned int length)
{
    memcpy(parsed_buffer, bytes, length);
    parsed_length = length;
    ++message_count;
}

int check_buffer_equal(const uint8_t* a, unsigned int length)
{
    if(length != parsed_length)
    {
        return 0;
    }

    for(unsigned int i = 0; i < length; ++i)
    {
        if(parsed_buffer[i] != a[i])
        {
            return 0;
        }
    }
    return 1;
}

TEST_INIT();

void check_passing_message_piece_by_piece_works()
{
    protocol_init();
    message_count = 0;

    uint8_t testmessage[] = {
        PROTOCOL_START_CODE,
        32, // length
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32
    };

    for(size_t i = 0; i < sizeof(testmessage); ++i)
    {
        protocol_feed_bytes(testmessage+i, 1);
    }

    TEST_ASSERT(check_buffer_equal(testmessage, sizeof(testmessage)));
    TEST_ASSERT_EQ(1, message_count);
}

void check_passing_message_in_one_go_works()
{
    protocol_init();
    message_count = 0;

    uint8_t testmessage[] = {
        PROTOCOL_START_CODE,
        32, // length
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32
    };

    protocol_feed_bytes(testmessage, sizeof(testmessage));

    TEST_ASSERT(check_buffer_equal(testmessage, sizeof(testmessage)));
    TEST_ASSERT_EQ(1, message_count);
}

void check_passing_multiple_messages()
{
    protocol_init();
    message_count = 0;

    uint8_t testmessage[] = {
        PROTOCOL_START_CODE,
        32, // length
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
        PROTOCOL_START_CODE,
        32, // length
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32
    };

    protocol_feed_bytes(testmessage, sizeof(testmessage));

    TEST_ASSERT(check_buffer_equal(testmessage+(sizeof(testmessage)/2), sizeof(testmessage)/2));
    TEST_ASSERT_EQ(2, message_count);
}

void check_passing_message_with_junk_works()
{
    protocol_init();
    message_count = 0;

    uint8_t testmessage[] = {
        0xdd,0xdd,0xdd,0xdd,
        PROTOCOL_START_CODE,
        32, // length
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32
    };

    protocol_feed_bytes(testmessage, sizeof(testmessage));

    TEST_ASSERT(check_buffer_equal(testmessage+4, sizeof(testmessage)-4));
    TEST_ASSERT_EQ(1, message_count);
}

void check_passing_message_piece_by_piece_with_junk_works()
{
    protocol_init();
    message_count = 0;

    uint8_t testmessage[] = {
        0xdd,0xdd,0xdd,0xdd,
        PROTOCOL_START_CODE,
        32, // length
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32
    };

    for(size_t i = 0; i < sizeof(testmessage); ++i)
    {
        protocol_feed_bytes(testmessage+i, 1);
    }

    TEST_ASSERT(check_buffer_equal(testmessage+4, sizeof(testmessage)-4));
    TEST_ASSERT_EQ(1, message_count);
}

int main(int argc, char** argv)
{
    TEST_FUNCTION(check_passing_message_piece_by_piece_works);
    TEST_FUNCTION(check_passing_message_in_one_go_works);
    TEST_FUNCTION(check_passing_multiple_messages);
    TEST_FUNCTION(check_passing_message_with_junk_works);
    TEST_FUNCTION(check_passing_message_piece_by_piece_with_junk_works);
    
    TEST_END();
}