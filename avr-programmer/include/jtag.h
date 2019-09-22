/******************************************************************************
 * z80-board-firmware
 * jtag.h - jtag programmer functionality
 *
 *****************************************************************************/

#ifndef Z80_BOARD_JTAG_H
#define Z80_BOARD_JTAG_H

#include <stdint.h>
#include <stdbool.h>

void jtag_init();
void disable_jtag();
void jtag_clk();
void jtag_tms(bool set);
void jtag_tdi(bool set);
bool jtag_tdo();

#endif
