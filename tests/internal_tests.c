/******************************************************************************
 * z80-board-firmware
 * internal_tests.c - checks the internal mocks work as expected
 *
 *****************************************************************************/

#include "test_utils.h"

#include "avr/interrupt.h"

TEST_INIT();

void check_sei()
{
    SREG = 0x7f; // all bits set except I
    sei();
    TEST_ASSERT_EQ(SREG, 0xff);

    // now try with no bits set
    SREG = 0x00;
    sei();
    TEST_ASSERT_EQ(SREG, 0x80);
}

void check_cli()
{
    SREG = 0xff; // all bits set
    cli();
    TEST_ASSERT_EQ(SREG, 0x7f);

    // now try with no bits set except I
    SREG = 0x80;
    cli();
    TEST_ASSERT_EQ(SREG, 0x00);
}

int main(int argc, char** argv)
{
    TEST_FUNCTION(check_sei);
    TEST_FUNCTION(check_cli);
    
    TEST_END();
}