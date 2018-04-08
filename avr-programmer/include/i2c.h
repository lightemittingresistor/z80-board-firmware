/******************************************************************************
 * z80-board-firmware
 * i2c.h - i2c functionality
 *
 *****************************************************************************/

#ifndef Z80_BOARD_I2C_H
#define Z80_BOARD_I2C_H

#include <stdint.h>

void i2c_init();

int i2c_read(uint8_t address, uint8_t* buffer, uint8_t length);

uint8_t i2c_readbyte(uint8_t addr);

void i2c_write(uint8_t address, uint8_t* data, uint8_t length);

// used when you need to send a byte then read the response
int i2c_writeread(uint8_t address, uint8_t data, uint8_t* buffer, uint8_t length);

#endif
