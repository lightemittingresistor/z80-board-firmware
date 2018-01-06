/******************************************************************************
 * z80-board-firmware
 * comms-protocol.c - serial protocol handler for download protocol
 *
 *****************************************************************************/

#include "comms-protocol.h"
#include "serial.h"

#include <stdbool.h>
#include <string.h>

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

enum states
{
    state_idle,
};

#define COMMAND_BUFFER_SIZE 32

struct protocol_state
{
    enum states current_state;
    char command[COMMAND_BUFFER_SIZE + 1];
    int command_ptr;

    bool echo;
};

static struct protocol_state state;

struct command_entry
{
    const char* name;
    const char* helpstring;
    int length;
    void (*handler)(const char*,int);
};

// Helper to set up the fields needed
#define COMMAND_FIELDS(NAME,DESC) \
void command_ ##NAME (const char* args, int length);\
const char NAME ##_name[] PROGMEM = # NAME; \
const char NAME ##_desc[] PROGMEM = DESC

// subtract 1 from length since we don't care about the null character
#define COMMAND_TABLE_ENTRY(NAME) \
{\
    .name = NAME ##_name, \
    .helpstring = NAME ##_desc, \
    .length = sizeof(NAME ##_name)-1, \
    .handler = command_ ##NAME \
}

COMMAND_FIELDS(help,"Displays this help");
COMMAND_FIELDS(serialnumber,"Gets the serial number of the kit");

static const struct command_entry commands[] PROGMEM = 
{
    COMMAND_TABLE_ENTRY(help),
    COMMAND_TABLE_ENTRY(serialnumber),
    { .name = NULL }
};

void protocol_init()
{
    state.current_state = state_idle;
    state.command_ptr = 0;
    state.command[0] = '\0';
    state.echo = true;
}

static int find_space(char* buffer, int start, int end)
{
    for(int i = start; i < end; ++i)
    {
        if(buffer[i] == ' ')
        {
            return i;
        }
    }
    return end;
}

#include <stdio.h>

static bool compare_commands(char* buffer, int size, const char* command, int command_length)
{
    if(size != command_length)
    {
        return false;
    }
    return strncmp_P(buffer, command, size) == 0;
}

void protocol_process()
{
    if(state.command_ptr > 0)
    {
        int first_space = find_space(state.command, 0, state.command_ptr);
        bool foundHandler = false;
        for(const struct command_entry* current = commands; pgm_read_ptr(&current->name); ++current)
        {
            if(compare_commands(state.command, first_space, pgm_read_ptr(&current->name), pgm_read_word(&current->length)))
            {
                void (*handler)(const char*,int) = pgm_read_ptr(&current->handler);
                handler(state.command + first_space+1, state.command_ptr - first_space);
                foundHandler = true;
                break;
            }
        }

        if(!foundHandler)
        {
            static const char errormsg[] PROGMEM = "Unknown command ";
            serial_put_P((const unsigned char*)errormsg, sizeof(errormsg));
            serial_put((const unsigned char*)state.command, first_space);
            serial_putchar('\n');
        }
    }

    state.command_ptr = 0;
    state.command[0] = '\0';
    if(state.echo)
    {
        serial_putchar('>');
    }
}

void protocol_state_idle(unsigned char byte)
{
    if(state.echo)
    {
        serial_putchar(byte);
    }

    switch(byte)
    {
    case '\n':
        break;
    case '\r':
        {
            protocol_process();
        } break;
    default:
        {
            if(state.command_ptr < COMMAND_BUFFER_SIZE)
            {
                state.command[state.command_ptr] = byte;
                ++state.command_ptr;
                state.command[state.command_ptr] = '\0';
            }
            else
            {
                static const char buffer_errormsg[] PROGMEM= "\nCommand Buffer Overflow\n";
                serial_put_P((unsigned char*)buffer_errormsg, sizeof(buffer_errormsg));
                protocol_process();
            }
        }
    };
}

void protocol_handle(unsigned char byte)
{
    if(state.current_state == state_idle)
    {
        protocol_state_idle(byte);
    }
}

void command_help(const char* buffer, int paramcount)
{
    static const char header[] PROGMEM = 
        "z80-board command interface\n"
        "--------------------------------\n"
        "  command  |  description\n"
        "--------------------------------\n";
    serial_put_P((const unsigned char*)header, sizeof(header));
    for(const struct command_entry* current = commands; pgm_read_ptr(&current->name); ++current)
    {
        serial_put((const unsigned char*)"  ", 2);
        serial_put_P((const unsigned char*)pgm_read_ptr(&current->name), pgm_read_word(&current->length));
        // pad out to divider
        for(int i=pgm_read_word(&current->length); i < 9; ++i)
        {
            serial_putchar(' ');
        }
        serial_put((const unsigned char*)"| ", 2);
        serial_put_P((const unsigned char*)pgm_read_ptr(&current->helpstring), strlen_P(pgm_read_ptr(&current->helpstring)));
        serial_putchar('\n');
    }
}

void command_serialnumber(const char* buffer, int paramcount)
{
    static const char header[] PROGMEM = 
        "Kit Serial Number: ";
    serial_put_P((const unsigned char*)header, sizeof(header));

    char pbuffer[32];
    sprintf(pbuffer, "%ld\n",eeprom_read_dword(0));
    serial_put((unsigned char*)pbuffer, strlen(pbuffer));
}
