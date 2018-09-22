/******************************************************************************
 * z80-board-firmware
 * usb.c - VUSB handling code
 *
 *****************************************************************************/

#ifdef ENABLE_VUSB
#include <usbdrv.h>
#else
#error VUSB Disabled!
#endif

PROGMEM const char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};

uchar usbFunctionRead(uchar *data, uchar len)
{
    return len;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    return 0;
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    return 0;
}

void usb_putchar(unsigned char c)
{
    
}

unsigned int usb_get(unsigned char* buffer, unsigned int size)
{
    return 0;
}

void usb_put(const unsigned char* buffer, unsigned int size)
{
    
}

void usb_put_P(const unsigned char* buffer, unsigned int size)
{
    
}
