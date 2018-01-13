/******************************************************************************
 * z80-board-firmware
 * stringtools.c - string utilities
 *
 *****************************************************************************/

#include "stringtools.h"

int countHexDigits(unsigned long val, unsigned long* out_max_val)
{
    int count = 0;
    unsigned long currentPow = 1;

    // 0 is a bit of a special case
    if(val == 0)
    {
        if(out_max_val)
        {
            *out_max_val = 1;
        }
        return 1;
    }

    while(currentPow <= val)
    {
        ++count;
        currentPow = currentPow << 4;
    }

    if(out_max_val)
    {
        *out_max_val = currentPow >> 4;
    }

    return count;
}

char intToHexDigit(unsigned char val)
{
    if(val < 10)
    {
        return '0' + val;
    }

    if(val < 16)
    {
        return 'a' + (val-10);
    }
    else
    {
        // that's not valid!
        return '?';
    }
}

void uinttohexstring(unsigned int val, char* buffer, int bufferlength)
{
    unsigned long max_col_val;
    int digitcount = countHexDigits(val, &max_col_val);

    if(bufferlength < (digitcount+1))
    {
        if(bufferlength >= 1)
        {
            buffer[0] = '\0';
        }
        return;
    }

    int col = 0;
    while(col < bufferlength && col < digitcount)
    {
        buffer[col] = intToHexDigit(val/max_col_val);
        val %= max_col_val;
        max_col_val = max_col_val >> 4;
        ++col;
    }

    buffer[digitcount] = '\0';
}

short hexdigittoint(char digit)
{
    if(digit >= '0' && digit <= '9')
    {
        return digit - '0';
    }
    if(digit >= 'a' && digit <= 'f')
    {
        return digit-'a'+10;
    }
    if(digit >= 'A' && digit <= 'F')
    {
        return digit-'A'+10;
    }
    return 0;
}