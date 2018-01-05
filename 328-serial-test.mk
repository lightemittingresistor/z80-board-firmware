

TARGET=atmega328p
FREQUENCY=16000000

328_SERIAL_TEST_SOURCES = \
	src/test/328-serial-test.c \
	src/comms-protocol.c \
	src/serial.c


328_SERIAL_TEST_OBJECTS_TEMP = $(328_SERIAL_TEST_SOURCES:src/%=build/%)
328_SERIAL_TEST_OBJECTS = $(328_SERIAL_TEST_OBJECTS_TEMP:.c=.o)


build/328-serial-test.elf: ${328_SERIAL_TEST_OBJECTS}
	${CC} ${CFLAGS} -Wl,-Map,$(@:.elf=.map) $^ -o $@
	avr-size --format=avr --mcu=${TARGET} $@

328-serial-test: build/328-serial-test.hex build/328-serial-test.lst

