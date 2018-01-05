/******************************************************************************
 * z80-board-firmware
 * comms-protocol.c - serial protocol handler for download protocol
 *
 *****************************************************************************/

#include "comms-protocol.h"
#include "serial.h"

#include <stdbool.h>
#include <string.h>

// TODO: Make this work with PROGMEM
//#include <avr/pgmspace.h>

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

void command_help(const char* args, int length);

static const struct command_entry commands[] = 
{
    {
        .name = "help",
        .helpstring = "Displays this help",
        .length = 4,
        .handler = command_help
    },
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
        char msg[128];
        sprintf(msg, "Length %d does not match %d\n", size, command_length);
        serial_put((unsigned char*)msg, strlen(msg));
        return false;
    }
    return strncmp(buffer, command, size) == 0;
}

void protocol_process()
{
    if(state.command_ptr > 0)
    {
        int first_space = find_space(state.command, 0, state.command_ptr);
        bool foundHandler = false;
        for(const struct command_entry* current = commands; current->name; ++current)
        {
            if(compare_commands(state.command, first_space, current->name, current->length))
            {
                current->handler(state.command + first_space+1, state.command_ptr - first_space);
                foundHandler = true;
                break;
            }
        }

        if(!foundHandler)
        {
            static const char errormsg[] = "Unknown command ";
            serial_put((const unsigned char*)errormsg, sizeof(errormsg));
            serial_put((const unsigned char*)state.command, first_space);
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
                static const char buffer_errormsg[] = "Command Buffer Overflow\n";
                serial_put((unsigned char*)buffer_errormsg, sizeof(buffer_errormsg));
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
    static const char header[] = 
        "z80-board command interface\n"
        "--------------------------------\n"
        "  command  |  description\n"
        "--------------------------------\n";
    serial_put((const unsigned char*)header, sizeof(header));
    for(const struct command_entry* current = commands; current->name; ++current)
    {
        serial_put((const unsigned char*)"  ", 2);
        serial_put((const unsigned char*)current->name, current->length);
        // pad out to divider
        for(int i=current->length; i < 9; ++i)
        {
            serial_putchar(' ');
        }
        serial_put((const unsigned char*)"| ", 2);
        serial_put((const unsigned char*)current->helpstring, strlen(current->helpstring));
        serial_putchar('\n');
    }
}