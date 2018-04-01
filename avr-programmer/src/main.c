/******************************************************************************
 * z80-board-firmware
 * main.c - main entry point
 *
 *****************************************************************************/

#include "serial.h"
#include "memorybus.h"
#include "comms-protocol.h"

#include <avr/interrupt.h>

int main()
{
    memory_init();
    serial_init(57600);
    protocol_init();

    sei();

    while(1)
    {
        protocol_handle(serial_getchar());
    }

    return 0;
}