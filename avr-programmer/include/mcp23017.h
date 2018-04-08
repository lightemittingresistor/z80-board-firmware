/******************************************************************************
 * z80-board-firmware
 * mcp23017.h - Interface for i2c 16-port port expander
 *
 * Note: This module assumes BANK is 0 at startup
 *****************************************************************************/

#ifndef Z80_BOARD_MCP23017_H
#define Z80_BOARD_MCP23017_H

#include <stdint.h>

#define MCP23017_PORTA 0
#define MCP23017_PORTB 1

#define MCP23017_PIN(PIN) (1 << ((PIN)-1))

// takes pin argument for consistency and in case this somehow were to be swapped
#define MCP23017_INPUT(PIN) (1 << ((PIN)-1))
#define MCP23017_OUTPUT(PIN) 0

// Takes the three bit digit assigned via pins
#define MCP23017_ADDRESS(ADDR) (0x20 | ((ADDR) & 0x07)) 

// Sets the direction of the pins
void mcp23017_set_direction(uint8_t address, int port, uint8_t direction);

// Inverts the logic level of the pins
void mcp23017_invert_pins(uint8_t address, int port, uint8_t pins);

// Enables pullups on the pins
void mcp23017_set_pullups(uint8_t address, int port, uint8_t pins);

// Read from the output port
uint8_t mcp23017_read_port(uint8_t address, int port);

// Read from the output latch
uint8_t mcp23017_read_latch(uint8_t address, int port);

void mcp23017_write(uint8_t address, int port, uint8_t data);

#endif
