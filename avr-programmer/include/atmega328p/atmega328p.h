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

// Port D control lines
#define BUSACK 7
#define BUSREQ 6
#define WRITEMODE 5
#define RD 4
#define WR 3
#define MREQ 2

static inline long addressbus_read()
{
    return 0;//(PINC << 8) | PINA;
}

static inline unsigned char databus_read()
{
    return 0;//((PINE & 0x07) << 5) | (PINB & 0x1F);
}

static inline void addressbus_set(long address)
{
    if(!busmaster) return;

    //PORTA = address & 0xFF;
    //PORTC = (address >> 8);
}

static inline void databus_set(unsigned char data)
{
    if(!busmaster || !datamaster) return;

    //PORTE = (PORTE & 0xF8) | ((data >> 5) & 0x07);
    //PORTB = data & 0x1F;
}

static inline void controllines_writemode(bool mode)
{
    if(mode)
    {
        //PORTD |= (1 << WRITEMODE);
    }
    else
    {
        //PORTD &= ~(1 << WRITEMODE);
    }
}

static inline void controllines_read(bool read)
{
    if(!busmaster) return;
    if(read)
    {
        //PORTD |= (1 << RD);
    }
    else
    {
        //PORTD &= ~(1 << RD);
    }
}

static inline void controllines_write(bool write)
{
    if(!busmaster) return;
    if(write)
    {
        //PORTD |= (1 << WR);
    }
    else
    {
        //PORTD &= ~(1 << WR);
    }
}

static inline void controllines_memreq(bool memreq)
{
    if(!busmaster) return;
    if(memreq)
    {
        //PORTD |= (1 << MREQ);
    }
    else
    {
        //PORTD &= ~(1 << MREQ);
    }
}

static inline void controllines_busreq(bool busreq)
{
    if(busreq)
    {
        //PORTD |= (1 << BUSREQ);
    }
    else
    {
        //PORTD &= ~(1 << BUSREQ);
    }
}

static inline bool controllines_busack()
{
    return 0;//(PIND & (1 << BUSACK)) == (1 << BUSACK);
}

#endif
