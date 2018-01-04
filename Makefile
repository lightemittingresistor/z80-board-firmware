# Makefile for z80 board firmware

TARGET=atmega8515

PROJECT=z80-board-firmware

SOURCES = \
	src/main.c \
	src/${TARGET}.c

CC=avr-gcc
AS=avr-as
OBJDUMP=avr-objdump
OBJCOPY=avr-objcopy

CFLAGS=-mmcu=${TARGET} -g -O2 -I$(CURDIR)/include

# three stages to replace different parts of the filename
OBJECTS_TEMP = $(SOURCES:src/%=build/%)
OBJECTS_C = $(OBJECTS_TEMP:.c=.o)
OBJECTS = $(OBJECTS_C:.S=.o)

all: build/${PROJECT}.elf build/${PROJECT}.lst build/${PROJECT}.hex

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

build/${PROJECT}.elf: ${OBJECTS}
	${CC} ${CFLAGS} -Wl,-Map,$(@:.elf=.map) $< -o $@
	avr-size --format=sysv $@

clean:
	rm -f build/*