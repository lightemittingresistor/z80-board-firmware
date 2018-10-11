#include <z80-board/z80-board.h>

#define pio_a_d 0x0
#define pio_b_d 0x01
#define pio_a_c 0x02
#define pio_b_c 0x03

// { NUM STEPS, STEP1.. STEPN },
const char sequence0[] = {8, 1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7 };
const char sequence1[] = {14, 1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 6, 1 << 5, 1 << 4, 1 << 3, 1 << 2, 1 << 1 };
const char sequence2[] = {2, 0xf0, 0x0f };
const char sequence3[] = {4, 128+1,64+2,32+4,16+8 };
const char sequence4[] = {6, 128+1,64+2,32+4,16+8, 32+4, 64+2};
const char sequence5[] = {2, 0x55, 0xaa };
const char sequence6[] = {9, 0xff, 0xff - 1, 0xff - 2, 0xff - 4, 0xff - 8, 0xff - 16, 0xff - 32, 0xff - 64, 0xff - 128 };
const char sequence7[] = {8, 1, 1 << 3, 1 << 4, 1 << 1, 1 << 2, 1 << 5, 1 << 7, 1 << 6 };

char* currentSequence = sequence0;
int sequencePos = 0;

void setSequence(int s)
{
	switch(s)
	{
	case 0:
		currentSequence = sequence0;
		break;
	case 1:
		currentSequence = sequence1;
		break;
	case 2:
		currentSequence = sequence2;
		break;
	case 3:
		currentSequence = sequence3;
		break;
	case 4:
		currentSequence = sequence4;
		break;
	case 5:
		currentSequence = sequence5;
		break;
	case 6:
		currentSequence = sequence6;
		break;
	case 7:
		currentSequence = sequence7;
		break;
	}
}

void checkKeys()
{
	int keys = getPins();
	int i;
	int val = 1;
	for(i=0; i < 8; ++i)
	{
		if(val == keys)
		{
			setSequence(i);
			sequencePos = 0;
			return;
		}
		val = (val << 1) & 0xff;
	}
}

void wait()
{
	unsigned int i = 0xff;
	unsigned int j = 0xff;
	while(j-- > 0)
	{
		i = 0xff;
		while(i-- > 0);
		checkKeys();
	}
}

void setup()
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
	#endasm
}

void main()
{
	
	setup();
	while(1)
	{	
		setPins(currentSequence[sequencePos+1]);
		++sequencePos;
		if(sequencePos >= currentSequence[0])
		{
			sequencePos = 0;
		}
		wait();
	}
	return;
}