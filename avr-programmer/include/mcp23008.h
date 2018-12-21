/******************************************************************************
 * z80-board-firmware
 * mcp23008.h - Interface for i2c 8-bit port expander
 *
 *****************************************************************************/

#ifndef Z80_BOARD_MCP23008_H
#define Z80_BOARD_MCP23008_H

#include <stdint.h>

#define MCP23008_CONFIG_INTPOL (1 << 1)
#define MCP23008_CONFIG_ODR (1 << 2)
#define MCP23008_CONFIG_DISSLW (1 << 4)
#define MCP23008_CONFIG_SEQOP (1 << 5)

// Takes the three bit digit assigned via pins
#define MCP23008_ADDRESS(ADDR) (0x20 | ((ADDR) & 0x07)) 

// Sets the direction of the pins
void mcp23008_set_direction(uint8_t address, uint8_t direction);

// Sets a flag for each pin whether it is inverted or not
void mcp23008_invert_pins(uint8_t address, uint8_t pins);

void mcp23008_set_config(uint8_t address, uint8_t config);
uint8_t mcp23008_get_config(uint8_t address);

// Enables pullups on the pins
void mcp23008_set_pullups(uint8_t address, uint8_t pins);

// Read from the output port
uint8_t mcp23008_read_port(uint8_t address);

// Read from the output latch
uint8_t mcp23008_read_latch(uint8_t address);

void mcp23008_write(uint8_t address, uint8_t data);

#endif
