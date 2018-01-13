

TARGET=atmega328p
FREQUENCY=16000000

328_SERIAL_TEST_SOURCES = \
	src/test/328-serial-test.c \
	src/test/serial-rxtest.c \
	src/comms-protocol.c \
	src/stringtools.c \
	src/hex-receiver.c \
	src/serial.c


328_SERIAL_TEST_OBJECTS_TEMP = $(328_SERIAL_TEST_SOURCES:src/%=build/%)
328_SERIAL_TEST_OBJECTS = $(328_SERIAL_TEST_OBJECTS_TEMP:.c=.o)


build/328-serial-test.elf: ${328_SERIAL_TEST_OBJECTS}
	${CC} ${CFLAGS} -Wl,-Map,$(@:.elf=.map) $^ -o $@
	avr-size --format=avr --mcu=${TARGET} $@

328-serial-test: build/328-serial-test.hex build/328-serial-test.lst

328flash : build/328-serial-test.hex
	avrdude -p atmega328p -c arduino -P /dev/tty.usbserial-A400hvpr -b 57600 -U flash:w:build/328-serial-test.hex