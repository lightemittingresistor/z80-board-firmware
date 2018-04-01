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

    databus_input();
    addressbus_set(address);
    _delay_us(10);
    controllines_memreq(false); // active high, remember
    controllines_read(false);
    _delay_us(10);
    unsigned char retval = databus_read();
    controllines_read(true);

    databus_idle();

    return retval;
}

void memory_write(long address, unsigned char data)
{
    if(!busmaster) return;

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
}

void memory_writemultiple(unsigned long startAddress, unsigned char* data, unsigned long length)
{
    if(!busmaster) return;

    databus_output();
    controllines_memreq(false);

    unsigned long counter = 0;

    while(counter < length)
    {
        unsigned long sectionStartAddress = startAddress + counter;
        unsigned long currentAddress = sectionStartAddress;
        while((sectionStartAddress & 0xff00) == (currentAddress & 0xff00) 
                && counter < length)
        {
            addressbus_set(currentAddress++);
            databus_set(data[counter++]);
            controllines_write(false);
            _delay_us(1);
            controllines_write(true);
        }

        // wait for write to happen
        while(memory_read(currentAddress-1) != data[counter-1]);
    }

    databus_idle();
}
