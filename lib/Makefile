

all: build/test.ihx build/z80-board.lib

build/z80-board.lib: z80-board.lst
	mkdir -p build
	zcc +z80 -clib=new  -x @$<    -o $@ 

build/test.ihx: test.c build/z80-board.lib
	zcc +z80 -clib=new  $< -m -create-app -startup=1 -o $@ -Cz"--ihex"  -pragma-define:REGISTER_SP=0xffff -Lbuild -lz80-board -Iinclude

clean:
	rm -f build/z80-board.lib
	rm -f build/test.*