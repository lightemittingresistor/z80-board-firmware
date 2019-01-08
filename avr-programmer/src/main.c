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

    // test code to zero out memory
    /*static const int step = 128;
    static const int value = 0;
    unsigned char buffer[step];
    memset(buffer, value, step);
    memory_takebus();
    for(uint16_t i = 0; i < 8192; i += step)
    {
        memory_writemultiple(i, buffer, step);
    }
    DEBUG_LOG_STRING("Done zeroing");
    memory_releasebus();*/

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