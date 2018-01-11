# Makfile fragment for PC-based tests

INTERNAL_TEST_SOURCES = \
	tests/internal_tests.c \
	$(CURDIR)/tests/mock_include/mock_include.c

TESTS_CFLAGS = -g -DF_CPU=${FREQUENCY}UL -DPC_TEST -I$(CURDIR)/include -I$(CURDIR)/tests/mock_include -Wall -Werror

TEST_CC=gcc

OBJECTS_INTERNAL_TEST = $(INTERNAL_TEST_SOURCES:%.c=build-tests/%.o)

build-tests/%.o : %.c
	mkdir -p `dirname $@`
	${TEST_CC} -c ${TESTS_CFLAGS} $< -o $@

build-tests/internal_tests : ${OBJECTS_INTERNAL_TEST}
	${TEST_CC}  ${TESTS_CFLAGS} -o $@ $^

internal_tests: build-tests/internal_tests
	build-tests/internal_tests

check : internal_tests