/******************************************************************************
 * z80-board-firmware
 * atmega328p.c - device-specific functions
 *
 *****************************************************************************/

#include "device.h"
#include "i2c.h"

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
    // Set BUSREQ high, WriteMode low
    // (BUSREQ initially to pullup actually which is fine)
    //PORTD = (1 << BUSREQ);

    // Port D 7 input (BUSACK)
    // Port D 6,5 output (BUSREQ, WriteMode)
    // Port D 4,3 input (RD,WR)
    // Port D 2 input (MREQ)
    //DDRD = (1 << BUSREQ) | (1 << WRITEMODE);
}

void controllines_becomebusmaster()
{
    // address bus all outputs
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTA, 0x00);
    mcp23017_set_direction(addressbus_portexpander, MCP23017_PORTB, 0x00);

    // RD,WR,MREQ output (start high)
    //PORTD |= (1 << RD) | (1 << WR) | (1 << MREQ);
    //DDRD |= (1 << RD) | (1 << WR) | (1 << MREQ);

    databus_idle();
}

void controllines_dropbusmaster()
{
    // Address bus inputs (without pullup)
    //DDRA = 0x00; DDRC = 0x00;
    //PORTA = 0x00; PORTC = 0x00;

    // RD,WR,MREQ input (without pullups)
    //PORTD &= ~((1 << RD) | (1 << WR) | (1 << MREQ));
    //DDRD &= ~((1 << RD) | (1 << WR) | (1 << MREQ));

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