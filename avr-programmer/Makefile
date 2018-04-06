# Makefile for z80 board firmware (AVR)

# Currently targetting an ATmega8515 clocked at 8MHz
ifeq ($(TARGET),)
TARGET=atmega8515
endif

ifeq ("$(TARGET)","atmega8515")

AVRDUDE_TARGET=m8515
ENABLE_USB=0
ifeq ($(FREQUENCY),)
FREQUENCY=8000000
endif

else
ifeq ("$(TARGET)","atmega328p")

AVRDUDE_TARGET=m328p
ENABLE_USB=0
ifeq ($(FREQUENCY),)
FREQUENCY=16000000
endif

else
$(error unknown TARGET)
endif
endif

PROJECT=z80-board-firmware

SOURCES = \
	src/main.c \
	src/memorybus.c \
	src/comms-protocol.c \
	src/stringtools.c \
	src/hex-receiver.c \
	src/${TARGET}.c

CC=avr-gcc
AS=avr-as
OBJDUMP=avr-objdump
OBJCOPY=avr-objcopy

# -O3 is great but inlines some chunky functions with LTO therefore making the code huge!
CFLAGS=-mmcu=${TARGET} -DF_CPU=${FREQUENCY}UL -g -O2 -I$(CURDIR)/include -Wall -Werror \
		-Wl,--gc-sections -flto -I$(CURDIR)/include/$(TARGET)

ifeq ($(ENABLE_USB), 1)
SOURCES += \
	src/usb.c \
	vusb-20121206/usbdrv/usbdrv.c \
	vusb-20121206/usbdrv/usbdrvasm.S \
	vusb-20121206/usbdrv/oddebug.c
CFLAGS += -I$(CURDIR)/vusb-20121206/usbdrv/ -DENABLE_VUSB -DDEBUG_LEVEL=1
else
SOURCES += \
	src/serial.c
endif

# three stages to replace different parts of the filename
OBJECTS_TEMP = $(SOURCES:%=build/%)
OBJECTS_C = $(OBJECTS_TEMP:.c=.o)
OBJECTS = $(OBJECTS_C:.S=.o)

all: build/${PROJECT}-${TARGET}.lst build/${PROJECT}-${TARGET}.hex

include tests.mk

build/%.o : %.c
	mkdir -p `dirname $@`
	${CC} -c ${CFLAGS} $< -o $@

build/%.o : %.S
	mkdir -p `dirname $@`
	${CC} -c ${CFLAGS} $< -o $@

%.lst : %.elf
	${OBJDUMP} -h -S $< > $@

%.hex : %.elf
	${OBJCOPY} -j .text -j .data -O ihex $< $@
	${OBJCOPY} -j .eeprom -O ihex $< ${@:%.hex=%-eeprom.hex}

build/${PROJECT}-${TARGET}.elf: ${OBJECTS}
	${CC} ${CFLAGS} -Wl,-Map,$(@:.elf=.map) $^ -o $@
	avr-size --format=avr --mcu=${TARGET} $@

flash : build/${PROJECT}-${TARGET}.hex
	avrdude -p ${AVRDUDE_TARGET} -c usbasp -U flash:w:$< -U lfuse:w:0xEF:m -U hfuse:w:0xD1:m

clean:
	rm -rf build/*
	rm -rf build-tests/*