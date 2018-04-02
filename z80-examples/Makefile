# Makefile for z80 board firmware (z80 code)

ASM=zasm

all: build/example.hex build/example.rom

build/%.s: %.s
	mkdir -p `dirname $@`
	cp $< $@

%.hex: %.s
	${ASM} -x -w -u $<

%.rom: %.s
	${ASM} -b -l0 $<

clean:
	rm -rf build