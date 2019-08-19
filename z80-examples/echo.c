/* echo.c
 * 
 * Echo example for Z80 Board - makes the LEDs follow the buttons
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
        in a, (pio_a_d)
        out (pio_a_d), a
        #endasm

    }

}