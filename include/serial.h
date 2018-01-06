/******************************************************************************
 * z80-board-firmware
 * serial.h - serial I/O functions
 *
 *****************************************************************************/

#ifndef Z80_BOARD_SERIAL_H
#define Z80_BOARD_SERIAL_H

#include <avr/io.h>
#include <stdbool.h>

void serial_init(long baud);

#ifdef __AVR_ATmega328P__
#define RXC RXC0
#define TXEN TXEN0
#define RXEN RXEN0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define URSEL UCSZ01
#define UCSZ1 UCSZ01
#define UCSZ0 UCSZ00
#define UDR UDR0
#define UDRE UDRE0

#endif

static inline bool serial_data_available()
{
    return (UCSRA & (1 << RXC)) == (1 << RXC);
}

// blocking call - get next available serial character
char serial_getchar();

// send a single character
void serial_putchar(unsigned char c);

// takes a buffer and buffer size.
// Returns the actual amount of data received.
// Does not block if no data is available.
unsigned int serial_get(unsigned char* buffer, unsigned int size);

// send the contents of a buffer
void serial_put(const unsigned char* buffer, unsigned int size);

// like serial_put but where buffer is in flash space
void serial_put_P(const unsigned char* buffer, unsigned int size);

#endif
