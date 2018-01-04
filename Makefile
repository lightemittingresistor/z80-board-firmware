# Makefile for z80 board firmware

TARGET=atmega8515

CC=avr-gcc
OBJDUMP=avr-objdump
OBJCOPY=avr-objcopy

CFLAGS=-mmcu=${TARGET} -g -O2 -I$(CURDIR)/include

SOURCES = \
	src/main.c \
	src/${TARGET}.c

# two stages to replace different parts of the filename
OBJECTS_TEMP = $(SOURCES:src/%=build/%)
OBJECTS = $(OBJECTS_TEMP:.c=.o)

all: build/z80-board-firmware.elf build/z80-board-firmware.lst build/z80-board-firmware.hex

build/%.o : src/%.c
	mkdir -p `dirname $@`
	${CC} -c ${CFLAGS} $< -o $@

%.lst : %.elf
	${OBJDUMP} -h -S $< > $@

%.hex : %.elf
	${OBJCOPY} -j .text -j .data -O ihex $< $@

build/z80-board-firmware.elf: ${OBJECTS}
	${CC} ${CFLAGS} -Wl,-Map,$(@:.elf=.map) $< -o $@
	avr-size --format=sysv $@

clean:
	rm -f build/*