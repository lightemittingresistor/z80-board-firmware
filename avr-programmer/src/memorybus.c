/******************************************************************************
 * z80-board-firmware
 * memory.c - memory bus functions
 *
 *****************************************************************************/

#include "memorybus.h"
#include "device.h"
#include "debug.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

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
    DEBUG_LOG_STRING("Taking memory bus");
    // store interrupt state
    unsigned char stored_sreg = SREG;
    cli();

    if(busmaster) return;

    // busreq - active low
    controllines_busreq(false);

    // wait for busack to go low
    while(controllines_busack()) wdt_reset();

    controllines_becomebusmaster();

    busmaster = true;

    //active high signals
    controllines_read(true);
    controllines_write(true);
    controllines_memreq(true);

    SREG = stored_sreg;
    DEBUG_LOG_STRING("Taken memory bus");
}

void memory_releasebus()
{
    DEBUG_LOG_STRING("Releasing memory bus");
    // store interrupt state
    unsigned char stored_sreg = SREG;
    cli();

    if(!busmaster) return;

    controllines_dropbusmaster();

    // deassert busreq - active low
    controllines_busreq(true);

    busmaster = false;

    SREG = stored_sreg;
    DEBUG_LOG_STRING("Released memory bus");
}

static unsigned char memory_read_internal(long address, bool io)
{
    if(!busmaster) return 0x00;

    databus_input();
    addressbus_set(address);
    _delay_us(10);
    if(io)
    {
        controllines_ioreq(false); // active low, remember
    }
    else
    {
        controllines_memreq(false); // active low, remember
    }
    controllines_read(false);
    _delay_us(10);
    unsigned char retval = databus_read();
    controllines_read(true);

    if(io)
    {
        controllines_ioreq(true);
    }
    else
    {
        controllines_memreq(true);
    }

    databus_idle();

    return retval;
}

unsigned char memory_read(long address)
{
    return memory_read_internal(address, false);
}

void memory_write_internal(long address, unsigned char data, bool io)
{
    if(!busmaster) return;

    // I don't think we're clocking fast enough to need waits here
    databus_output();
    addressbus_set(address);
    
    if(io)
    {
        controllines_ioreq(false);
    }
    else
    {
        controllines_memreq(false);
    }

    databus_set(data);
    controllines_write(false);
    controllines_write(true);

    if(io)
    {
        controllines_ioreq(true);
    }
    else
    {
        controllines_memreq(true);
    }

    databus_idle();

}

void memory_writeprotect_dance()
{
    memory_write_internal(0x1555, 0xaa, false);
    memory_write_internal(0x0aaa, 0x55, false);
    memory_write_internal(0x1555, 0xa0, false);
}

void memory_write(long address, unsigned char data)
{
    if(!busmaster) return;

    // if we're in ROM, do write protect dance
    if(address < 0x8000)
    {
        memory_writeprotect_dance();
    }

    memory_write_internal(address, data, false);

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

        // If in ROM, handle write protection
        if(currentAddress < 0x8000)
        {
            memory_writeprotect_dance();
            databus_output();
        }

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

#ifdef BOARD_SUPPORTS_IOREQ
unsigned char memory_io_read(long address)
{
    return memory_read_internal(address, true);
}

void memory_io_write(long address, unsigned char data)
{
    if(!busmaster) return;

    memory_write_internal(address, data, true);
}
#endif
