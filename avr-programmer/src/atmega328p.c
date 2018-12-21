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

#ifdef DEBUG_ENABLED
// init serial as well for debug
#include "serial.h"
#endif

#else
#include "serial.h"
#endif

#include <avr/io.h>

void device_init()
{
#ifdef ENABLE_VUSB
    odDebugInit();
    usbInit();
#ifdef DEBUG_ENABLED
    // init serial as well for debug
    serial_init(57600);
#endif

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
    // no inversions
    mcp23008_invert_pins(databus_portexpander, 0);
    // sequential operation disabled, slew rate control enabled
    mcp23008_set_config(databus_portexpander, MCP23008_CONFIG_SEQOP | MCP23008_CONFIG_DISSLW);

    // configre as idle to start
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
    DDRC |= (1 << BUSREQ);
}

void controllines_becomebusmaster()
{
    DEBUG_LOG_STRING("Taking control of busses");
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
    DEBUG_LOG_STRING("Dropping control of busses");

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
    if(!busmaster) return;

    // all pins output
    mcp23008_set_direction(databus_portexpander, 0x00);

    datamaster = true;
}

void databus_input()
{
    if(!busmaster) return;

    // all pins input (with pullup)
    mcp23008_set_pullups(databus_portexpander, 0xff);
    mcp23008_set_direction(databus_portexpander, 0xff);

    datamaster = false;
}

void databus_idle()
{
    // input (no pullup) effectively tristates the lines
    mcp23008_set_pullups(databus_portexpander, 0x00);
    mcp23008_set_direction(databus_portexpander, 0xff);

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