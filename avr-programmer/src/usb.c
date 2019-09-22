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

#include "debug.h"
#include "usb.h"
#include "memorybus.h"
#include "jtag.h"

#include <string.h>
#include <util/delay.h>

PROGMEM const char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x00,                    //   REPORT_SIZE (0)
    0x95, 0x00,                    //   REPORT_COUNT (0)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};

// Scratch buffer for small requests
unsigned char scratch[64];
uint16_t currentPointer = 0;
uint16_t remainingBytes = 0;
bool expectingPointer = false;

usbMsgLen_t handleStatusRequest(usbRequest_t* rq)
{
    DEBUG_LOG_STRING("handleStatusRequest");

    if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE)
    {
        DEBUG_LOG_STRING("Why are you sending me status?");
        return 0;
    }

    // HW Version (2.0 -> 2,0)
    scratch[0] = 2;
    scratch[1] = 0;

    // Software version (1.7 -> 1,7)
    scratch[2] = 0;
    scratch[3] = 1;

    // are we currently master?
    scratch[4] = busmaster ? 1 : 0;

    usbMsgPtr = (usbMsgPtr_t)scratch;

    return 5; // returning 5 bytes
}

usbMsgLen_t handleTakeControlRequest(usbRequest_t* rq)
{
    DEBUG_LOG_STRING("handleTakeControlRequest");

    if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_DEVICE_TO_HOST)
    {
        scratch[0] = busmaster ? 1 : 0;
        usbMsgPtr = (usbMsgPtr_t)scratch;
        return 1;
    }

    if(rq->wValue.bytes[0] == 1)
    {
        memory_takebus();
    }
    else
    {
        memory_releasebus();
    }

    DEBUG_LOG_STRING("handleTakeControlRequest exit");

    return 0;
}

usbMsgLen_t handlePointerRequest(usbRequest_t* rq)
{
    DEBUG_LOG_STRING("handlePointerRequest");

    if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_DEVICE_TO_HOST)
    {
        scratch[0] = currentPointer & 0xff;
        scratch[1] = (currentPointer & 0xff00) >> 8;
        return 2;
    }

    expectingPointer = true;
    
    return USB_NO_MSG;
}
usbMsgLen_t handleTransferRequest(usbRequest_t* rq)
{
    DEBUG_LOG_STRING("handleTransferRequest");
    if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_DEVICE_TO_HOST)
    {
        remainingBytes = rq->wValue.word;
        DEBUG_LOG_VAL("Reading bytes - count: ", remainingBytes);
    }

    if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE)
    {
        remainingBytes = rq->wValue.word;
        DEBUG_LOG_VAL("Writing bytes - count: ", remainingBytes);
    }

    return USB_NO_MSG;
}

usbMsgLen_t handleResetRequest(usbRequest_t* rq)
{
    DEBUG_LOG_STRING("handleResetRequest");

    controlllines_reset(true);
    _delay_ms(100);
    controlllines_reset(false);

    return 0;
}

usbMsgLen_t handleJtagRequest(usbRequest_t* rq)
{
    if(rq->wValue.bytes[0] == 1)
    {
        controlllines_reset(true);
        jtag_init();
    }
    else
    {
        disable_jtag();
        controlllines_reset(false);
    }

    return 0;
}

usbMsgLen_t handleJtagIORequest(usbRequest_t* rq)
{
    //DEBUG_LOG_STRING("handleJtagIORequest");

    bool tms = (rq->wValue.bytes[0] & 1) == 1;
    bool tdi = (rq->wValue.bytes[0] & 2) == 2;

    scratch[0] = jtag_tdo() ? 1 : 0;

    jtag_tms(tms);
    jtag_tdi(tdi);
    jtag_clk();

    usbMsgPtr = (usbMsgPtr_t)scratch;
    return 1;
}

uchar usbFunctionRead(uchar *data, uchar len)
{
    //DEBUG_LOG_VAL("usbFunctionRead: ", len);
    
    if(len > remainingBytes)
    {
        len = remainingBytes;
    }

    for(int i=0; i<len; ++i)
    {
        data[i] = memory_read(currentPointer+i);
    }

    currentPointer += len;
    remainingBytes -= len;

    if(remainingBytes == 0)
    {
        DEBUG_LOG_STRING("Read Complete");
    }

    return len;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    //DEBUG_LOG_VAL("usbFunctionWrite: ", len);

    if(expectingPointer == true && len >= 2)
    {
        currentPointer = data[1] << 8;
        currentPointer |= data[0];
        expectingPointer = false;
        DEBUG_LOG_VAL("New Pointer: ", currentPointer);
        return 2;
    }

    if(len > remainingBytes)
    {
        len = remainingBytes;
    }

    //memory_writemultiple(currentPointer, data, len);

    for(uint8_t i = 0; i < len; ++i)
    {
        memory_write(currentPointer+i, data[i]);
    }

    currentPointer += len;
    remainingBytes -= len;

    if(remainingBytes == 0)
    {
        DEBUG_LOG_STRING("Write Complete");
    }
    else
    {
        //DEBUG_LOG_STRING("Write Ongoing");
    }

    return len;
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;
    rq = rq;

    //DEBUG_LOG_STRING("usbFunctionSetup");
    //DEBUG_LOG_VAL("Request Type: ", rq->bmRequestType);
    //DEBUG_LOG_VAL("Request: ", rq->bRequest);
    //DEBUG_LOG_VAL("Index: ", rq->wIndex.word);
    //DEBUG_LOG_VAL("Value: ", rq->wValue.word);
    //DEBUG_LOG_VAL("Length: ", rq->wLength.word);

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_VENDOR)
    {
        switch(rq->bRequest)
        {
            case USB_REQ_GET_STATUS:
            {
                return handleStatusRequest(rq);
            } break;
            case USB_REQ_TAKE_CONTROL:
            {
                return handleTakeControlRequest(rq);
            } break;
            case USB_REQ_POINTER:
            {
                return handlePointerRequest(rq);
            } break;
            case USB_REQ_TRANSFER_DATA:
            {
                return handleTransferRequest(rq);
            } break;
            case USB_REQ_RESET:
            {
                return handleResetRequest(rq);
            } break;
            case USB_REQ_JTAG:
            {
                return handleJtagRequest(rq);
            } break;
            case USB_REQ_JTAG_IO:
            {
                return handleJtagIORequest(rq);
            } break;
        }
    }
    return 0;
}

