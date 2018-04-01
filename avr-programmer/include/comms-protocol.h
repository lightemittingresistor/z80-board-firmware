/******************************************************************************
 * z80-board-firmware
 * comms-protocol.h - serial protocol handler for download protocol
 *
 *****************************************************************************/

#ifndef Z80_BOARD_DEVICE_COMMS_PROTOCOL_H
#define Z80_BOARD_DEVICE_COMMS_PROTOCOL_H

void protocol_init();
void protocol_handle(unsigned char byte);

#endif
