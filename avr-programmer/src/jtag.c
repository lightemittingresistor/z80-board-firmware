/******************************************************************************
 * z80-board-firmware
 * jtag.c - jtag interface for programming CPLD
 *
 *****************************************************************************/

#include "jtag.h"
#include "device.h"

#include <util/delay.h>

void jtag_init()
{
    // TDO as input, TDI, TCK, TMS outputs, no pullups
    JTAG_PORT &= ~((1 << JTAG_TDO) | (1 << JTAG_TDI) | (1 << JTAG_TCK) | (1 << JTAG_TMS));
    JTAG_DDR &= ~(1 << JTAG_TDO);
    JTAG_DDR |= (1 << JTAG_TDI) | (1 << JTAG_TCK) | (1 << JTAG_TMS);

}

void disable_jtag()
{
    // all inputs, no pullups (effectively disable it)
    JTAG_PORT &= ~((1 << JTAG_TDO) | (1 << JTAG_TDI) | (1 << JTAG_TCK) | (1 << JTAG_TMS));
    JTAG_DDR &= ~((1 << JTAG_TDO) | (1 << JTAG_TDI) | (1 << JTAG_TCK) | (1 << JTAG_TMS));
}

void jtag_clk()
{
    JTAG_PORT |= (1 << JTAG_TCK);
    _delay_us(10);
    JTAG_PORT &= ~(1 << JTAG_TCK);
}

void jtag_tms(bool set)
{
    if(set)
    {
        JTAG_PORT |= (1 << JTAG_TMS);
    }
    else
    {
        JTAG_PORT &= ~(1 << JTAG_TMS);
    }
}

bool jtag_tdo()
{
    return (JTAG_PIN & (1 << JTAG_TDO)) == (1 << JTAG_TDO);
}
