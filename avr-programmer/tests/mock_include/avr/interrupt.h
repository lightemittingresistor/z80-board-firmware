/******************************************************************************
 * z80-board-firmware
 * avr/interrupt.h - mock header for tests
 *
 *****************************************************************************/

#ifndef Z80_BOARD_TEST_MOCK_INTERRUPT_H
#define Z80_BOARD_TEST_MOCK_INTERRUPT_H

#include "io.h"

#define sei() (SREG |= (1 << 7))
#define cli() (SREG &= ~(1 << 7))

#endif
