/******************************************************************************
 * z80-board-firmware
 * avr/io.h - mock header for tests
 *
 *****************************************************************************/

#ifndef Z80_BOARD_TEST_MOCK_IO_H
#define Z80_BOARD_TEST_MOCK_IO_H

// mock registers
extern unsigned char UBRRH;
extern unsigned char UBRRL;
extern unsigned char UCSRA;
extern unsigned char UCSRB;
extern unsigned char UCSRC;
extern unsigned char UDR;
extern unsigned char SREG;

// UCSRA bits
#define RXC 7
#define TXC 6
#define UDRE 5
#define FE 4
#define DOR 3
#define PE 2
#define U2X 1
#define MPCM 0

// UCSRB bits
#define RXCIE 7
#define TXCIE 6
#define UDRIE 5
#define RXEN 4
#define TXEN 3
#define UCSZ2 2
#define RXB8 1
#define TXB8 0

// UCSRC bits
#define URSEL 7
#define UMSEL 6
#define UPM1 5
#define UPM0 5
#define USBS 3
#define UCSZ1 2
#define UCSZ0 1
#define UCPOL 0


#endif
