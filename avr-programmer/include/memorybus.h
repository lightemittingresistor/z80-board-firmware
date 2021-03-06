/******************************************************************************
 * z80-board-firmware
 * memorybus.h - Memory bus functions
 *
 *****************************************************************************/

#ifndef Z80_BOARD_MEMORY_H
#define Z80_BOARD_MEMORY_H

#include "device.h"
#include <stdbool.h>

// flag to determine if we are in control of the busses or not
extern bool busmaster;

// when busmaster is set, datamaster determines if we are sending or receiving
extern bool datamaster;

void memory_init();

// take control of the memory bus
void memory_takebus();

// Drop control of the memory bus
void memory_releasebus();

unsigned char memory_read(long address);
void memory_write(long address, unsigned char data);
void memory_writemultiple(unsigned long startAddress, unsigned char* data, unsigned long length);

//#ifdef BOARD_SUPPORTS_IOREQ
unsigned char memory_io_read(long address);
void memory_io_write(long address, unsigned char data);
//#endif

#endif
