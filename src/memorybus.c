/******************************************************************************
 * z80-board-firmware
 * memory.c - memory bus functions
 *
 *****************************************************************************/

#include "memorybus.h"
#include "device.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdbool.h>

bool busmaster = false;
bool datamaster = false;

void memory_init()
{
    addressbus_init();
    databus_init();
    controllines_init();
}

void memory_takebus()
{
    // store interrupt state
    unsigned char stored_sreg = SREG;
    cli();

    if(busmaster) return;

    // busreq - active low
    controllines_busreq(false);

    // wait for busack to go low
    while(controllines_busack());

    controllines_becomebusmaster();

    busmaster = true;

    //active high signals
    controllines_read(true);
    controllines_write(true);
    controllines_memreq(true);

    SREG = stored_sreg;
}

void memory_releasebus()
{
    // store interrupt state
    unsigned char stored_sreg = SREG;
    cli();

    if(!busmaster) return;

    controllines_dropbusmaster();

    // deassert busreq - active low
    controllines_busreq(true);

    busmaster = false;

    SREG = stored_sreg;
}

unsigned char memory_read(long address)
{
    if(!busmaster) return 0x00;

    // store interrupt state
    unsigned char stored_sreg = SREG;
    cli();

    databus_input();
    addressbus_set(address);
    _delay_us(1);
    controllines_memreq(false); // active high, remember
    controllines_read(false);
    _delay_us(1);
    unsigned char retval = databus_read();
    controllines_read(true);

    databus_idle();

    SREG = stored_sreg;

    return retval;
}

void memory_write(long address, unsigned char data)
{
    if(!busmaster) return;

    // store interrupt state
    unsigned char stored_sreg = SREG;
    cli();

    // I don't think we're clocking fast enough to need waits here
    databus_output();
    addressbus_set(address);
    controllines_memreq(false);
    databus_set(data);
    controllines_write(false);
    controllines_write(true);

    databus_idle();

    // wait for write to happen
    while(memory_read(address) != data);

    SREG = stored_sreg;
}