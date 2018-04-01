# Makefile for z80 board firmware

all: avr avr-check

avr:
	${MAKE} -C avr-programmer

avr-check:
	${MAKE} -C avr-programmer check

clean:
	${MAKE} -C avr-programmer clean