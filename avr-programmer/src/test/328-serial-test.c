/******************************************************************************
 * z80-board-firmware
 * 328-serial-test.c - prove out serial code on an arduino board
 *
 *****************************************************************************/

#include "serial.h"
#include "comms-protocol.h"

#include <avr/interrupt.h>

void rxts_havebyte(char byte);

void memory_takebus() {}
void memory_write(long address, unsigned char data) {}
unsigned char memory_read(long address) { return 0x42; }
void memory_releasebus() {}


int main()
{
    serial_init(57600);
    protocol_init();

    sei();

    while(1)
    {
        protocol_handle(serial_getchar());
    }

    return 0;
}