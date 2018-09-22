# Makfile fragment for PC-based tests

INTERNAL_TEST_SOURCES = \
	tests/internal_tests.c \
	$(CURDIR)/tests/mock_include/mock_include.c

STRINGTOOLS_TEST_SOURCES = \
	tests/stringtools_tests.c \
	src/stringtools.c

HEX_PARSER_TEST_SOURCES = \
	tests/hex_parser_tests.c \
	src/hex-receiver.c \
	src/stringtools.c

PROTOCOL_ASSEMBLER_TESTS_SOURCES = \
	tests/protocol_assembler_tests.c \
	src/protocol/protocol_assembler.c \
	src/protocol/protocol.c

PROTOCOL_TESTS_SOURCES = \
	tests/protocol_tests.c \
	src/protocol/protocol_assembler.c \
	src/protocol/protocol.c

TESTS_CFLAGS = -g -DF_CPU=${FREQUENCY}UL -DPC_TEST -I$(CURDIR)/include \
				-I$(CURDIR)/tests/mock_include -Wall -Werror

TEST_CC=gcc

OBJECTS_INTERNAL_TEST = $(INTERNAL_TEST_SOURCES:%.c=build-tests/%.o)
OBJECTS_STRINGTOOLS_TEST = $(STRINGTOOLS_TEST_SOURCES:%.c=build-tests/%.o)
OBJECTS_HEX_PARSER_TEST = $(HEX_PARSER_TEST_SOURCES:%.c=build-tests/%.o)
OBJECTS_PROTOCOL_ASSEMBLER_TEST = $(PROTOCOL_ASSEMBLER_TESTS_SOURCES:%.c=build-tests/%.o)
OBJECTS_PROTOCOL_TEST = $(PROTOCOL_TESTS_SOURCES:%.c=build-tests/%.o)

build-tests/%.o : %.c
	mkdir -p `dirname $@`
	${TEST_CC} -c ${TESTS_CFLAGS} $< -o $@

build-tests/internal_tests : ${OBJECTS_INTERNAL_TEST}
	${TEST_CC}  ${TESTS_CFLAGS} -o $@ $^

build-tests/stringtools_tests : ${OBJECTS_STRINGTOOLS_TEST}
	${TEST_CC}  ${TESTS_CFLAGS} -o $@ $^

build-tests/hex_parser_tests : ${OBJECTS_HEX_PARSER_TEST}
	${TEST_CC}  ${TESTS_CFLAGS} -o $@ $^

build-tests/protocol_assembler_tests : ${OBJECTS_PROTOCOL_ASSEMBLER_TEST}
	${TEST_CC}  ${TESTS_CFLAGS} -o $@ $^	

build-tests/protocol_tests : ${OBJECTS_PROTOCOL_TEST}
	${TEST_CC}  ${TESTS_CFLAGS} -o $@ $^	

internal_tests: build-tests/internal_tests
	build-tests/internal_tests

stringtools_tests: build-tests/stringtools_tests
	build-tests/stringtools_tests

hex_parser_tests: build-tests/hex_parser_tests
	build-tests/hex_parser_tests

protocol_assembler_tests: build-tests/protocol_assembler_tests
	build-tests/protocol_assembler_tests

protocol_tests: build-tests/protocol_tests
	build-tests/protocol_tests

check : internal_tests stringtools_tests hex_parser_tests protocol_assembler_tests protocol_tests