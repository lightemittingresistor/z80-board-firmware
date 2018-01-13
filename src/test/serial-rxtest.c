#include "serial.h"
#include "stringtools.h"
#include <avr/pgmspace.h>

typedef struct
{
    char expected;
    int correct_count;
    int incorrect_count;
} rxtest_status;

rxtest_status rxts = { 0,0,0 };

void rxts_havebyte(char byte)
{
    if(byte == rxts.expected)
    {
        ++ rxts.correct_count;
    }
    else
    {
        ++ rxts.incorrect_count;
    }
    rxts.expected = byte+1;
    if(rxts.expected > '9')
    {
        rxts.expected = '0';
    }

    if(rxts.correct_count + rxts.incorrect_count > 1000)
    {
        static const char text1[] PROGMEM = 
            "As Expected: ";
        serial_put_P((unsigned char*)text1, sizeof(text1));

        char scratch[32];
        uinttohexstring(rxts.correct_count,scratch, 32);
        serial_put((unsigned char*)scratch, 32);
        serial_putchar('\n');

        static const char text2[] PROGMEM = 
            "Not As Expected: ";
        serial_put_P((unsigned char*)text2, sizeof(text2));

        uinttohexstring(rxts.incorrect_count,scratch, 32);
        serial_put((unsigned char*)scratch, 32);
        serial_putchar('\n');
        rxts.correct_count = 0;
        rxts.incorrect_count = 0;
    }
}