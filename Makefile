# Makefile for z80 board firmware

# Currently targetting an ATmega8515 clocked at 8MHz
TARGET=atmega8515
FREQUENCY=8000000

PROJECT=z80-board-firmware

SOURCES = \
	src/main.c \
	src/serial.c \
	src/memorybus.c \
	src/comms-protocol.c \
	src/${TARGET}.c


CC=avr-gcc
AS=avr-as
OBJDUMP=avr-objdump
OBJCOPY=avr-objcopy

CFLAGS=-mmcu=${TARGET} -DF_CPU=${FREQUENCY}UL -g -O3 -I$(CURDIR)/include -Wall -Werror \
		-Wl,--gc-sections -flto

# three stages to replace different parts of the filename
OBJECTS_TEMP = $(SOURCES:src/%=build/%)
OBJECTS_C = $(OBJECTS_TEMP:.c=.o)
OBJECTS = $(OBJECTS_C:.S=.o)

ifdef ENABLE_328
all: 328-serial-test
include 328-serial-test.mk
else
all: build/${PROJECT}.lst build/${PROJECT}.hex
endif

include tests.mk

build/%.o : src/%.c
	mkdir -p `dirname $@`
	${CC} -c ${CFLAGS} $< -o $@

build/%.o : src/%.S
	mkdir -p `dirname $@`
	${AS} -mmcu=${TARGET} $< -o $@

%.lst : %.elf
	${OBJDUMP} -h -S $< > $@

%.hex : %.elf
	${OBJCOPY} -j .text -j .data -O ihex $< $@
	${OBJCOPY} -j .eeprom -O ihex $< ${@:%.hex=%-eeprom.hex}

build/${PROJECT}.elf: ${OBJECTS}
	${CC} ${CFLAGS} -Wl,-Map,$(@:.elf=.map) $^ -o $@
	avr-size --format=avr --mcu=${TARGET} $@

flash : build/${PROJECT}.hex
	avrdude -p m8515 -c usbasp -U flash:w:build/${PROJECT}.hex -U lfuse:w:0xEF:m -U hfuse:w:0xD9:m

clean:
	rm -rf build/*
	rm -rf build-tests/*