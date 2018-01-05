/******************************************************************************
 * z80-board-firmware
 * serial_tests.c - Serial I/O functions
 *
 *****************************************************************************/

#include "test_utils.h"

#include "serial.h"

#include "avr/io.h"

TEST_INIT();

long calculate_ubrr(long baud);

#define CHECK_BAUD(EXPECTED) TEST_ASSERT_EQ(UBRRH,((EXPECTED) >> 8)); \
                                TEST_ASSERT_EQ(UBRRL,((EXPECTED) & 0xff))

void check_baud_calculations()
{
    // taken from datasheet
    #if F_CPU == 8000000UL
    serial_init(2400);
    CHECK_BAUD(207);
    serial_init(4800);
    CHECK_BAUD(103);
    serial_init(9600);
    CHECK_BAUD(51);
    // datasheet says 34 - 33 fits the algorithm more
    serial_init(14400);
    CHECK_BAUD(33);
    serial_init(19200);
    CHECK_BAUD(25);
    serial_init(28800);
    CHECK_BAUD(16);
    serial_init(38400);
    CHECK_BAUD(12);
    // datasheet says 8 - rounding down gets to 7 (actual is 8.68)
    serial_init(57600);
    CHECK_BAUD(7);
    // datahsheet says 6 - again, it rounds down to 5
    serial_init(76800);
    CHECK_BAUD(5);
    serial_init(115200);
    CHECK_BAUD(3);
    serial_init(230400);
    CHECK_BAUD(1);
    serial_init(250000);
    CHECK_BAUD(1);
    serial_init(500000);
    CHECK_BAUD(0);
    #else
    #error No values for F_CPU
    #endif
}

void check_flags()
{
    UCSRA = 0;
    UCSRB = 0;

    serial_init(9600);

    TEST_ASSERT(UCSRB & (1 << TXEN));
    TEST_ASSERT(UCSRB & (1 << RXEN));

    // 8,N,1
    TEST_ASSERT(UCSRC & (1 << URSEL));
    TEST_ASSERT(UCSRC & ((1 << UCSZ1) | (1 << UCSZ0)));

    // Async (UMSEL -> 0)
    TEST_ASSERT(!(UCSRC & (1 << UMSEL)));
    // No parity (UPM 00)
    TEST_ASSERT(!(UCSRC & ((1 << UPM1) | (1 << UPM0))));
}

void check_data_available()
{
    UCSRA = (unsigned char)~(1 << RXC);
    TEST_ASSERT(!serial_data_available());
    UCSRA = (1 << RXC);
    TEST_ASSERT(serial_data_available());
}

void check_get_char()
{
    // set flag that data is available
    UCSRA |= (1 << RXC);

    UDR = 'x';
    TEST_ASSERT_EQ('x', serial_getchar());
}

void check_put_char()
{
    // set flag that data is available
    UCSRA |= (1 << UDRE);

    UDR = 0x00;
    serial_putchar('x');
    TEST_ASSERT_EQ('x', UDR);
}

void check_serial_get()
{
    // start with no data available
    UCSRA = 0;
    unsigned char data[255];
    size_t received = serial_get(data, 255);
    TEST_ASSERT_EQ(received, 0);

    // for now, only one byte can be sent with the fake
    UCSRA |= (1 << RXC);
    UDR = 'x';
    received = serial_get(data, 1);
    TEST_ASSERT_EQ(received, 1);

    TEST_ASSERT_EQ('x', data[0]);

    // now check a zero length buffer
    received = serial_get(NULL, 0);
    TEST_ASSERT_EQ(received, 0);
}

int main(int argc, char** argv)
{
    TEST_FUNCTION(check_baud_calculations);
    TEST_FUNCTION(check_flags);
    TEST_FUNCTION(check_data_available);
    TEST_FUNCTION(check_get_char);
    TEST_FUNCTION(check_put_char);
    TEST_FUNCTION(check_serial_get);
    
    TEST_END();
}