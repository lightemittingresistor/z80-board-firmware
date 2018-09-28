# Makefile for z80 board firmware (z80 code)

ASM=z80asm --cpu=z80

all: build/example.hex

.PRECIOUS: %.o %.bin build/%.s

build/%s: %s.m4
	mkdir -p build
	m4 $< > $@

%.o: %.s
	mkdir -p build
	${ASM} --output=$@ -m $< 

%.hex: %.bin
	appmake +hex --org 0 -b $< -o $@

%.bin: %.o
	${ASM} -m -b $< 

clean:
	rm -rf build