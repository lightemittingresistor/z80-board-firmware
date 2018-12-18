/******************************************************************************
 * z80-board-firmware
 * atmega328p.h - board specific definitions
 *
 *****************************************************************************/

#ifndef Z80_BOARD_DEVICE_ATMEGA328P_H
#define Z80_BOARD_DEVICE_ATMEGA328P_H

#ifndef Z80_BOARD_DEVICE_H
#error Include device.h not this file directly
#endif

#include "mcp23017.h"

// Port C control lines
#define BUSACK 3
#define BUSREQ 2
#define MREQ 1
#define IOREQ 0

// Port B control lines
#define RD 1
#define WR 0
#define RESET 2

// Port D Control Lines
#define JTAG_TDI 3
#define JTAG_TDO 5
#define JTAG_TCK 6
#define JTAG_TMS 7

static uint8_t addressbus_portexpander = MCP23017_ADDRESS(0);

#define I2C_PORT PORTC
#define I2C_PIN_SDA 4
#define I2C_PIN_SCL 5

#define BOARD_SUPPORTS_RESET 1
#define BOARD_SUPPORTS_IOREQ 1

static inline long addressbus_read()
{
    return (mcp23017_read_port(addressbus_portexpander, MCP23017_PORTB) << 8) |
                mcp23017_read_port(addressbus_portexpander, MCP23017_PORTA);
}

static inline unsigned char databus_read()
{
    return 0;//((PINE & 0x07) << 5) | (PINB & 0x1F);
}

static inline void addressbus_set(long address)
{
    if(!busmaster) return;

    mcp23017_write(addressbus_portexpander, MCP23017_PORTA, address & 0xFF);
    mcp23017_write(addressbus_portexpander, MCP23017_PORTB, (address >> 8));
}

static inline void databus_set(unsigned char data)
{
    if(!busmaster || !datamaster) return;

    //PORTE = (PORTE & 0xF8) | ((data >> 5) & 0x07);
    //PORTB = data & 0x1F;
}

static inline void controllines_read(bool read)
{
    if(!busmaster) return;
    if(read)
    {
        PORTB |= (1 << RD);
    }
    else
    {
        PORTB &= ~(1 << RD);
    }
}

static inline void controllines_write(bool write)
{
    if(!busmaster) return;
    if(write)
    {
        PORTB |= (1 << WR);
    }
    else
    {
        PORTB &= ~(1 << WR);
    }
}

static inline void controllines_memreq(bool memreq)
{
    if(!busmaster) return;
    if(memreq)
    {
        PORTC |= (1 << MREQ);
    }
    else
    {
        PORTC &= ~(1 << MREQ);
    }
}

static inline void controllines_ioreq(bool ioreq)
{
    if(!busmaster) return;
    if(ioreq)
    {
        PORTC |= (1 << IOREQ);
    }
    else
    {
        PORTC &= ~(1 << IOREQ);
    }
}

static inline void controllines_busreq(bool busreq)
{
    if(busreq)
    {
        PORTC |= (1 << BUSREQ);
    }
    else
    {
        PORTC &= ~(1 << BUSREQ);
    }
}

static inline bool controllines_busack()
{
    return (PINC & (1 << BUSACK)) == (1 << BUSACK);
}

#endif
