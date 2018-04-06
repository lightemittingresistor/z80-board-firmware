/******************************************************************************
 * z80-board-firmware
 * main.c - main entry point
 *
 *****************************************************************************/

#include "device.h"
#include "memorybus.h"
#include "comms-protocol.h"

#ifdef ENABLE_VUSB
#include <usbdrv.h>
#else 
#include "serial.h"
#endif

#include <avr/interrupt.h>

int main()
{
    device_init();
    memory_init();
    protocol_init();

    sei();

    while(1)
    {
#ifdef ENABLE_VUSB
        usbPoll();
#else
        protocol_handle(serial_getchar());
#endif
    }

    return 0;
}