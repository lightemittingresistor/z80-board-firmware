/* blinky-world.c
 * 
 * Hello World example for Z80 Board
 *
 * Compile with z88dk:
 *  zcc +z80 -clib=new  blinky-world.c  -m -create-app  -o blinky-world -Cz"--ihex"  -pragma-define:REGISTER_SP=0xffff
*/

#define pio_a_d 0x0
#define pio_b_d 0x01
#define pio_a_c 0x02
#define pio_b_c 0x03

void main()
{
	while(1)
	{	
		#asm

		ld a, 0xcf
    out (pio_a_c), a ; send 0xcf to A (control mode)
    ld a, 0x00
    out (pio_a_c), a ; set A to output (0)
    ld a, 0xcf
    out (pio_b_c), a ; send 0xcf to B (control mode)
    ld a, 0xff
    out (pio_b_c), a ; set B to input (1)

    ; disable interupts
    ld a, 0x07
    out (pio_a_c), a
    out (pio_b_c), a 

    ; turn LEDs on
    ld a, 0xff
    out (pio_a_d), a ; set outputs to on


		ld a, 0xff
		out (0x08), a
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