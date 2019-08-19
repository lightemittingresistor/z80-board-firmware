/******************************************************************************
 * z80-board-firmware
 * mcp23008.c - Interface for i2c 8-port port expander
 *
 *****************************************************************************/

#include "mcp23008.h"
#include "i2c.h"
#include "debug.h"

// MCP23008 registers
#define IODIR 0x00
#define IPOL 0x01
#define GPINTEN 0x02
#define DEFVAL 0x03
#define INTCON 0x04
#define IOCON 0x05
#define GPPU 0x06
#define INTF 0x07
#define INTCAP 0x08
#define GPIO 0x09
#define OLAT 0x0A

static inline void mcp23008_write_register(uint8_t address, uint8_t reg, uint8_t data)
{
    //DEBUG_LOG_VAL("MCP23008: Write reg: ", reg);
    //DEBUG_LOG_VAL("MCP23008: Write data: ", data);
    uint8_t buffer[2] = { reg, data };
    i2c_write(address, buffer, 2);
}

void mcp23008_set_direction(uint8_t address, uint8_t direction)
{
    mcp23008_write_register(address, IODIR, direction);
}

void mcp23008_invert_pins(uint8_t address, uint8_t pins)
{
    mcp23008_write_register(address, IPOL, pins);
}

void mcp23008_set_config(uint8_t address, uint8_t config)
{
    mcp23008_write_register(address, IOCON, config);
}

uint8_t mcp23008_get_config(uint8_t address)
{
    uint8_t data;
    i2c_writeread(address, IOCON, &data, 1);
    return data;
}

void mcp23008_set_pullups(uint8_t address, uint8_t pins)
{
    mcp23008_write_register(address, GPPU, pins);
}

uint8_t mcp23008_read_port(uint8_t address)
{
    uint8_t data;
    i2c_writeread(address, GPIO, &data, 1);
    return data;
}

uint8_t mcp23008_read_latch(uint8_t address)
{
    uint8_t data;
    i2c_writeread(address, OLAT, &data, 1);
    return data;
}

void mcp23008_write(uint8_t address, uint8_t data)
{
    mcp23008_write_register(address, GPIO, data);
}