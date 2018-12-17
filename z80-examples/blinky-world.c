/* blinky-world.c
 * 
 * Hello World example for Z80 Board
 *
 * Compile with z88dk:
 *  zcc +z80 -clib=new  blinky-world.c  -m -create-app  -o blinky-world -Cz"--ihex"  -pragma-define:REGISTER_SP=0xffff
*/

#define pio_a_d 0x0

void main()
{
	while(1)
	{	
		#asm

		
		ld a, 0x0f
		out (pio_a_d), a
		#endasm
		unsigned int i = 0xffff;
		while(i-- > 0);
		i = 0xffff;
		while(i-- > 0);
		i = 0xffff;
		while(i-- > 0);
		i = 0xffff;
		while(i-- > 0);
		#asm
		ld a, 0x00
		out (0x08), a
		ld a, 0xf0
		out (pio_a_d), a
		#endasm
		i = 0xffff;
		while(i-- > 0);
		i = 0xffff;
		while(i-- > 0);
		i = 0xffff;
		while(i-- > 0);
		i = 0xffff;
		while(i-- > 0);
	}
	return;
}