/******************************************************************************
 * z80-board-firmware
 * usb.h - VUSB handling code
 *
 *****************************************************************************/

#ifndef Z80_BOARD_USB_H
#define Z80_BOARD_USB_H

#ifndef ENABLE_VUSB
#error VUSB Disabled!
#endif

// send a single character
void usb_putchar(unsigned char c);

// takes a buffer and buffer size.
// Returns the actual amount of data received.
// Does not block if no data is available.
unsigned int usb_get(unsigned char* buffer, unsigned int size);

// send the contents of a buffer
void usb_put(const unsigned char* buffer, unsigned int size);

// like usb_put but where buffer is in flash space
void usb_put_P(const unsigned char* buffer, unsigned int size);

#endif
