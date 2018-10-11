# Makefile for z80 board firmware (z80 code)

ASM=z80asm --cpu=z80

ASM_SRCS = example.s.m4

ASM_TARGETS_TMP=$(ASM_SRCS:.s.m4=.ihx)
ASM_TARGETS=$(ASM_TARGETS_TMP:%=build/%)

# Single source C files
C_SRCS = blinky-world.c

C_TARGETS_TMP=$(C_SRCS:.c=.ihx)
C_TARGETS=$(C_TARGETS_TMP:%=build/%)

all: $(ASM_TARGETS) $(C_TARGETS)

.PRECIOUS: %.o %.bin build/%.s

build/%s: %s.m4
	mkdir -p build
	m4 $< > $@

build/%.ihx: %.c
	zcc +z80 -clib=new  $<  -m -create-app  -o $@ -Cz"--ihex"  -pragma-define:REGISTER_SP=0xffff

%.o: %.s
	mkdir -p build
	${ASM} --output=$@ -m $< 

%.ihx: %.bin
	appmake +hex --org 0 -b $< -o $@

%.bin: %.o
	${ASM} -m -b $< 

clean:
	rm -rf build