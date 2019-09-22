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

// USB Request Numbers
static const uint8_t USB_REQ_GET_STATUS = 1;
static const uint8_t USB_REQ_TAKE_CONTROL = 2;
static const uint8_t USB_REQ_POINTER = 3;
static const uint8_t USB_REQ_TRANSFER_DATA = 4;
static const uint8_t USB_REQ_RESET = 5;
static const uint8_t USB_REQ_JTAG = 6;
static const uint8_t USB_REQ_JTAG_IO = 7;


#endif
