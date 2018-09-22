/******************************************************************************
 * z80-board-firmware
 * protocol_tests.c - checks the message utilities work
 *
 *****************************************************************************/

#include "protocol/protocol.h"

#include "test_utils.h"

#include <stdint.h>

TEST_INIT();

// dummy - not used in this test
void protocol_feed_message(unsigned char* bytes, unsigned int length)
{

}

void check_checksum()
{
    uint8_t data[] = {
        0x01,0x01,0x01,0x01, 0xfa,
    };

    uint8_t sum = protocol_checksum(data, sizeof(data));
    TEST_ASSERT_EQ(0x00, sum);
    
    sum = protocol_checksum(data, sizeof(data)-1);
    TEST_ASSERT_EQ(0xfa, sum);
}

int main(int argc, char** argv)
{
    TEST_FUNCTION(check_checksum);
    
    TEST_END();
}