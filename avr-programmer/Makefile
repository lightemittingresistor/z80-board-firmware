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

OUTPUT_SUFFIX=

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
		-Wl,--gc-sections -flto -I$(CURDIR)/include/$(TARGET) -I$(CURDIR)/build

ifeq ("$(ENABLE_DEBUG)","1")
	CFLAGS += "-DDEBUG_ENABLED "
	OUTPUT_SUFFIX="-debug"
endif

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

ifeq ("$(TARGET)","atmega328p")
SOURCES += \
	src/i2c.c \
	src/mcp23017.c
endif

# three stages to replace different parts of the filename
OBJECTS_TEMP = $(SOURCES:%=build/${TARGET}${OUTPUT_SUFFIX}/%)
OBJECTS_C = $(OBJECTS_TEMP:.c=.o)
OBJECTS = $(OBJECTS_C:.S=.o)

all: build/${PROJECT}-${TARGET}${OUTPUT_SUFFIX}.lst build/${PROJECT}-${TARGET}${OUTPUT_SUFFIX}.hex

include tests.mk

build/build-config.h : makeconfigfile.sh
	./makeconfigfile.sh > $@

build/${TARGET}${OUTPUT_SUFFIX}/%.o : %.c build/build-config.h
	mkdir -p `dirname $@`
	${CC} -c ${CFLAGS} $< -o $@

build/${TARGET}${OUTPUT_SUFFIX}/%.o : %.S
	mkdir -p `dirname $@`
	${CC} -c ${CFLAGS} $< -o $@

%.lst : %.elf
	${OBJDUMP} -h -S $< > $@

%.hex : %.elf
	${OBJCOPY} -j .text -j .data -O ihex $< $@
	${OBJCOPY} -j .eeprom -O ihex $< ${@:%.hex=%-eeprom.hex}

build/${PROJECT}-${TARGET}${OUTPUT_SUFFIX}.elf: ${OBJECTS}
	${CC} ${CFLAGS} -Wl,-Map,$(@:.elf=.map) $^ -o $@
	avr-size --format=avr --mcu=${TARGET} $@ | tee build/${PROJECT}-${TARGET}${OUTPUT_SUFFIX}.txt

flash : build/${PROJECT}-${TARGET}.hex
	avrdude -p ${AVRDUDE_TARGET} -c usbasp -U flash:w:$< -U lfuse:w:0xEF:m -U hfuse:w:0xD1:m

clean:
	rm -rf build/*
	rm -rf build-tests/*

all-targets:
	${MAKE} clean
	${MAKE} build/build-config.h
	${MAKE} TARGET=atmega8515
	${MAKE} TARGET=atmega328p
	${MAKE} TARGET=atmega8515 ENABLE_DEBUG=1
	${MAKE} TARGET=atmega328p ENABLE_DEBUG=1

