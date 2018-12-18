/******************************************************************************
 * z80-board-firmware
 * atmega328p.c - device-specific functions
 *
 *****************************************************************************/

#include "device.h"
#include "i2c.h"
#include "debug.h"

#ifdef ENABLE_VUSB
#include <usbdrv.h>
#include <oddebug.h>
#else
#include "serial.h"
#endif

#include <avr/io.h>

void device_init()
{
#ifdef ENABLE_VUSB
    odDebugInit();
    DBG1(0x00, 0, 0);
    usbInit();
#else
    serial_init(57600);
#endif

    i2c_init();
}

void addressbus_init()
{
    // Expander port A all inputs (no pullup)
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTA, 0xff);
    mcp23017_set_pullups(addressbus_portexpander, MCP23017_PORTA, 0x00);

    // Expander port B all inputs (no pullup)
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTB, 0xff);
    mcp23017_set_pullups(addressbus_portexpander, MCP23017_PORTB, 0x00);
}

void databus_init()
{
    databus_idle();
}

void controllines_init()
{
    // Set BUSREQ high
    // (BUSREQ initially to pullup actually which is fine)
    PORTC |= (1 << BUSREQ);

    // RD,RW,Reset as inputs, no pullups
    DDRB &= ~((1 << RD) | (1 << WR) | (1 << RESET));
    PORTB &= ~((1 << RD) | (1 << WR) | (1 << RESET));

    // IOREQ, MREQ, BUSACK as inputs, no pullups
    DDRC &= ~((1 << IOREQ) | (1 << MREQ) | (1 << BUSACK));
    PORTC &= ~((1 << IOREQ) | (1 << MREQ) | (1 << BUSACK));
    
    //now enable pullups on BUSACK
    PORTC |= (1 << BUSACK);

    //BUSREQ is an output
    DDRC &= (1 << BUSREQ);
}

void controllines_becomebusmaster()
{
    // address bus all outputs
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTA, 0x00);
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTB, 0x00);

    // RD, WR output (start high)
    PORTB |= (1 << RD) | (1 << WR);
    DDRB  |= (1 << RD) | (1 << WR);

    // MREQ, IOREQ output (start high)
    PORTC |= (1 << MREQ) | (1 << IOREQ);
    DDRC  |= (1 << MREQ) | (1 << IOREQ);

    databus_idle();
}

void controllines_dropbusmaster()
{
    // Expander port A all inputs (no pullup)
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTA, 0xff);
    mcp23017_set_pullups(addressbus_portexpander, MCP23017_PORTA, 0x00);

    // Expander port B all inputs (no pullup)
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTB, 0xff);
    mcp23017_set_pullups(addressbus_portexpander, MCP23017_PORTB, 0x00);

    // RD,RW as inputs, no pullups
    DDRB &= ~((1 << RD) | (1 << WR));
    PORTB &= ~((1 << RD) | (1 << WR));

    // IOREQ, MREQ as inputs, no pullups
    DDRC &= ~((1 << IOREQ) | (1 << MREQ));
    PORTC &= ~((1 << IOREQ) | (1 << MREQ));

    databus_idle();
}

void databus_output()
{
    //if(!busmaster) return;

    //high bits - start with zero
    //PORTE &= 0xf8;
    //DDRE |= 0x07;

    // low bits
    //PORTB &= 0x1f;
    //DDRB = 0x1f;

    datamaster = true;
}

void databus_input()
{
    //if(!busmaster) return;

    // Port B 4:0 all inputs (with pullup)
    //DDRB = 0x00; PORTB = 0x1F;

    // Port E 2:0 all inputs (with pullup)
    //DDRE = 0x00; PORTE = 0x07;

    datamaster = false;
}

void databus_idle()
{
    // Port B 4:0 all inputs (no pullup)
    //DDRB = 0x00; PORTB = 0x00;

    // Port E 2:0 all inputs (no pullup)
    //DDRE = 0x00; PORTE = 0x00;

    datamaster = false;
}

void controlllines_reset(bool assert)
{
    if(assert)
    {
        DEBUG_LOG_STRING("Asserting RESET");
        // set PB2 as output
        DDRB |= (1 << 2);
        // set low
        PORTB &= ~(1 << 2);

    }
    else
    {
        DEBUG_LOG_STRING("Deasserting RESET");
        // basically return to inactive state
        // set PB2 as input
        DDRB &= ~(1 << 2);
        // set no pull up
        PORTB &= ~(1 << 2);
    }

    DEBUG_LOG_VAL("DDRB", DDRB);
}