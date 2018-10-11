# Makefile for z80 board firmware

all: avr avr-check z80 lib

avr:
	${MAKE} -C avr-programmer

avr-check:
	${MAKE} -C avr-programmer check

z80: lib
	${MAKE} -C z80-examples

lib:
	${MAKE} -C lib

clean:
	${MAKE} -C avr-programmer clean
	${MAKE} -C z80-examples clean