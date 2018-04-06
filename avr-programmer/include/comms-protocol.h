/******************************************************************************
 * z80-board-firmware
 * comms-protocol.h - serial protocol handler for download protocol
 *
 *****************************************************************************/

#ifndef Z80_BOARD_DEVICE_COMMS_PROTOCOL_H
#define Z80_BOARD_DEVICE_COMMS_PROTOCOL_H

#ifdef ENABLE_VUSB
#include "usb.h"
#else
#include "serial.h"
#endif

void protocol_init();
void protocol_handle(unsigned char byte);

static inline void comms_putchar(unsigned char c)
{
#ifdef ENABLE_VUSB
    usb_putchar(c);
#else
    serial_putchar(c);
#endif
}

static inline unsigned int comms_get(unsigned char* buffer, unsigned int size)
{
#ifdef ENABLE_VUSB
    return usb_get(buffer, size);
#else
    return serial_get(buffer, size);
#endif
}

static inline void comms_put(const unsigned char* buffer, unsigned int size)
{
#ifdef ENABLE_VUSB
    return usb_put(buffer, size);
#else
    return serial_put(buffer, size);
#endif
}

static inline void comms_put_P(const unsigned char* buffer, unsigned int size)
{
#ifdef ENABLE_VUSB
    return usb_put_P(buffer, size);
#else
    return serial_put_P(buffer, size);
#endif
}

#endif
