/******************************************************************************
 * z80-board-firmware
 * device.h - device-specific functions
 *
 *****************************************************************************/

#ifndef Z80_BOARD_DEVICE_H
#define Z80_BOARD_DEVICE_H

#include "memorybus.h"

#include <avr/io.h>

// Initialise the busses to their default state
// Assumes interrupts disabled
void addressbus_init();
void databus_init();
void controllines_init();

// Low level bus routines - not for use directly
// unless you are in memorybus.c!
void controllines_becomebusmaster();
void controllines_dropbusmaster();
static inline void controllines_writemode(bool mode);
static inline void controllines_read(bool read);
static inline void controllines_write(bool write);
static inline void controllines_memreq(bool memreq);
static inline void controllines_busreq(bool busreq);
static inline bool controllines_busack();

void databus_output();
void databus_input();
void databus_idle();
static inline unsigned char databus_read();
static inline void databus_set(unsigned char data);

static inline long addressbus_read();
static inline void addressbus_set(long address);


#if __AVR_ATmega8515__
#include "atmega8515.h"
#endif

#endif
