/******************************************************************************
 * z80-board-firmware
 * main.c - main entry point
 *
 *****************************************************************************/

#include "device.h"
#include "memorybus.h"
#include "protocol/protocol.h"
#include "debug.h"

#ifdef ENABLE_VUSB
#include <usbdrv.h>
#else 
#include "serial.h"
#endif

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include <string.h>

int main()
{
    wdt_enable(WDTO_1S);
    device_init();
    memory_init();
    protocol_init();

    sei();

    // if possible, reset the CPU to make sure it's in a known state
    #ifdef BOARD_SUPPORTS_RESET
    controlllines_reset(true);
    _delay_ms(100);
    controlllines_reset(false);
    #endif

    while(1)
    {
        wdt_reset();
#ifdef ENABLE_VUSB
        usbPoll();
#else
        uint8_t b = serial_getchar();
        protocol_feed_bytes(&b, 1);
#endif
    }

    return 0;
}