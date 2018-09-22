/******************************************************************************
 * z80-board-firmware
 * protocol_serial.c - Serial specific protocol stuff
 *
 *****************************************************************************/

#include "serial.h"

// ASCII characters because why not?
#define SERIAL_ACK 0x06
#define SERIAL_NAK 0x15

bool protocol_send_data(unsigned char* bytes, unsigned int length)
{
    serial_put(bytes, length);
    char result = serial_getchar();

    return result == SERIAL_ACK;
}

void protocol_send_ack()
{
    serial_putchar(SERIAL_ACK);
}

void protocol_send_nak()
{
    serial_putchar(SERIAL_NAK);
}