/******************************************************************************
 * z80-board-firmware
 * 328-serial-test.c - prove out serial code on an arduino board
 *
 *****************************************************************************/

#include "serial.h"
#include "comms-protocol.h"

int main()
{
    serial_init(9600);
    protocol_init();

    while(1)
    {
        protocol_handle(serial_getchar());
    }

    return 0;
}