/******************************************************************************
 * z80-board-firmware
 * mcp23017.h - Interface for i2c 16-port port expander
 *
 *****************************************************************************/

#include "mcp23017.h"
#include "i2c.h"

// MCP23017 registers
#define IODIRA 0x00
#define IODIRB 0x01
#define IPOLA 0x02
#define IPOLB 0x03
#define GPINTENA 0x04
#define GPINTENB 0x05
#define DEFVALA 0x06
#define DEFVALB 0x07
#define INTCONA 0x08
#define INTCONB 0x09
#define IOCON 0x0A
//#define IOCON 0B
#define GPPUA 0x0C
#define GPPUB 0x0D
#define INTFA 0x0E
#define INTFB 0x0F
#define INTCAPA 0x10
#define INTCAPB 0x11
#define GPIOA 0x12
#define GPIOB 0x13
#define OLATA 0x14
#define OLATB 0x15

static inline void mcp23017_write_register(uint8_t address, uint8_t reg, uint8_t data)
{
    uint8_t buffer[2] = { reg, data };
    i2c_write(address, buffer, 2);
}

void mcp23017_set_direction(uint8_t address, int port, uint8_t direction)
{
    uint8_t reg;
    if(port == MCP23017_PORTA)
    {
        reg = IODIRA;
    }
    else
    {
        reg = IODIRB;
    }

    mcp23017_write_register(address, reg, direction);
}

void mcp23017_invert_pins(uint8_t address, int port, uint8_t pins)
{
    uint8_t reg;
    if(port == MCP23017_PORTA)
    {
        reg = IPOLA;
    }
    else
    {
        reg = IPOLB;
    }

    mcp23017_write_register(address, reg, pins);
}

void mcp23017_set_pullups(uint8_t address, int port, uint8_t pins)
{
    uint8_t reg;
    if(port == MCP23017_PORTA)
    {
        reg = GPPUA;
    }
    else
    {
        reg = GPPUB;
    }

    mcp23017_write_register(address, reg, pins);
}

uint8_t mcp23017_read_port(uint8_t address, int port)
{
    uint8_t reg;
    if(port == MCP23017_PORTA)
    {
        reg = GPIOA;
    }
    else
    {
        reg = GPIOB;
    }
    uint8_t data;
    i2c_writeread(address, reg, &data, 1);
    return data;
}

uint8_t mcp23017_read_latch(uint8_t address, int port)
{
    uint8_t reg;
    if(port == MCP23017_PORTA)
    {
        reg = OLATA;
    }
    else
    {
        reg = OLATB;
    }
    uint8_t data;
    i2c_writeread(address, reg, &data, 1);
    return data;
}

void mcp23017_write(uint8_t address, int port, uint8_t data)
{
    uint8_t reg;
    if(port == MCP23017_PORTA)
    {
        reg = GPIOA;
    }
    else
    {
        reg = GPIOB;
    }

    mcp23017_write_register(address, reg, data);
}
