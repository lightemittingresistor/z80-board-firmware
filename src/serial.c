/******************************************************************************
 * z80-board-firmware
 * serial.c - Serial I/O functions
 *
 *****************************************************************************/

#include "serial.h"

#include <avr/interrupt.h>

#ifndef F_CPU
#error F_CPU must be defined for this to work
#endif

#include <avr/pgmspace.h>

// TODO: Use buffers for reliability and implement basic flow control

void serial_init(long baud)
{
    // store interrupt state
    unsigned char stored_sreg = SREG;
    cli();
    // set up baud rate
    const long ubrr = (F_CPU/(8*baud))-1;
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)(ubrr);

    //enable double clock rate
    UCSRA = (1 << U2X);

    //enable TX and RX (8 bit)
    UCSRB = (1 << RXEN) | (1 << TXEN);
    // 8,N,1
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    SREG = stored_sreg;
}

char serial_getchar()
{
    // wait for data
    while(!(UCSRA & (1 << RXC)));

    return UDR;
}

void serial_putchar(unsigned char c)
{
    if(c == '\n')
    {
        serial_putchar('\r');
    }
    // wait for any previous character to be sent
    while(!(UCSRA & (1 << UDRE)));
    UDR = c;
}

unsigned int serial_get(unsigned char* buffer, unsigned int size)
{
    unsigned int received = 0;

    while(UCSRA & (1 << RXC) && (received < size))
    {
        buffer[received++] = UDR;
    }

    return received;
}

void serial_put(const unsigned char* buffer, unsigned int size)
{
    for(unsigned int i = 0; i < size; ++i)
    {
        serial_putchar(buffer[i]);
    }
}

void serial_put_P(const unsigned char* buffer, unsigned int size)
{
    for(unsigned int i = 0; i < size; ++i)
    {
        serial_putchar(pgm_read_byte(buffer + i));
    }
}