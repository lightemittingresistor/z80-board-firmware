/******************************************************************************
 * z80-board-firmware
 * stringtools.h - String Utilities
 *
 *****************************************************************************/

#ifndef Z80_BOARD_STRINGTOOLS_H
#define Z80_BOARD_STRINGTOOLS_H

// Returns the number of hex digits in val.
// Optionally, out_max_val may be passed to get the column value of the
// most significant column
int countHexDigits(unsigned long val, unsigned long* out_max_val);

void uinttohexstring(unsigned int val, char* buffer, int bufferlength);

short hexdigittoint(char digit);

#endif
