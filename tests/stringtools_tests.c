/******************************************************************************
 * z80-board-firmware
 * stringtools_tests.c - checks the string tools work
 *
 *****************************************************************************/

#include "test_utils.h"

#include "stringtools.h"

#include <string.h>

TEST_INIT();

void check_digit_count()
{
    TEST_ASSERT_EQ(1, countHexDigits(0x0, NULL));
    TEST_ASSERT_EQ(1, countHexDigits(0x9, NULL));
    TEST_ASSERT_EQ(2, countHexDigits(0x10, NULL));
    TEST_ASSERT_EQ(2, countHexDigits(0x10, NULL));
    TEST_ASSERT_EQ(2, countHexDigits(0xff, NULL));
    TEST_ASSERT_EQ(3, countHexDigits(0x100, NULL));
    TEST_ASSERT_EQ(3, countHexDigits(0xfff, NULL));
}

void check_max_power()
{
    unsigned long maxpow;

    countHexDigits(0x0, &maxpow);
    TEST_ASSERT_EQ(1,maxpow);

    countHexDigits(0x1, &maxpow);
    TEST_ASSERT_EQ(1,maxpow);

    countHexDigits(0x10, &maxpow);
    TEST_ASSERT_EQ(0x10,maxpow);

    countHexDigits(0xff, &maxpow);
    TEST_ASSERT_EQ(0x10,maxpow);

    countHexDigits(0x100, &maxpow);
    TEST_ASSERT_EQ(0x100,maxpow);

    countHexDigits(0xfff, &maxpow);
    TEST_ASSERT_EQ(0x100,maxpow);
}

void check_hex_to_string()
{
    char buffer[32];

    uinttohexstring(0x1, buffer, 32);
    TEST_ASSERT_EQ(0, strcmp("1", buffer));

    uinttohexstring(0xf, buffer, 32);
    TEST_ASSERT_EQ(0, strcmp("f", buffer));

    uinttohexstring(0x10, buffer, 32);
    TEST_ASSERT_EQ(0, strcmp("10", buffer));

    uinttohexstring(0xff, buffer, 32);
    TEST_ASSERT_EQ(0, strcmp("ff", buffer));

    uinttohexstring(0x100, buffer, 32);
    TEST_ASSERT_EQ(0, strcmp("100", buffer));
}

void check_hex_digit_to_string()
{
    TEST_ASSERT_EQ(0, hexdigittoint('0'));
    TEST_ASSERT_EQ(9, hexdigittoint('9'));
    TEST_ASSERT_EQ(10, hexdigittoint('a'));
    TEST_ASSERT_EQ(15, hexdigittoint('f'));
    TEST_ASSERT_EQ(10, hexdigittoint('A'));
    TEST_ASSERT_EQ(15, hexdigittoint('F'));
}

int main(int argc, char** argv)
{
    TEST_FUNCTION(check_digit_count);
    TEST_FUNCTION(check_max_power);
    TEST_FUNCTION(check_hex_to_string);
    TEST_FUNCTION(check_hex_digit_to_string);
    
    TEST_END();
}