/******************************************************************************
 * z80-board-firmware
 * comms-protocol.c - serial protocol handler for download protocol
 *
 *****************************************************************************/

#include "comms-protocol.h"
#include "memorybus.h"
#include "serial.h"

#include <stdbool.h>
#include <string.h>

#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

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
COMMAND_FIELDS(reset,"Resets the interface");
COMMAND_FIELDS(peek,"peek");
COMMAND_FIELDS(poke,"poke");

static const struct command_entry commands[] PROGMEM = 
{
    COMMAND_TABLE_ENTRY(help),
    COMMAND_TABLE_ENTRY(serialnumber),
    COMMAND_TABLE_ENTRY(reset),
    COMMAND_TABLE_ENTRY(peek),
    COMMAND_TABLE_ENTRY(poke),
    { .name = NULL }
};

void protocol_startup()
{
    static const char welcomemsg[] PROGMEM =
        "z80-board command interface\n"
        "Started.\n"
        ">";
    serial_put_P((const unsigned char*)welcomemsg, sizeof(welcomemsg));
}

void protocol_init()
{
    // might have been turned on by reset
    MCUCSR = 0;
    wdt_disable();

    state.current_state = state_idle;
    state.command_ptr = 0;
    state.command[0] = '\0';
    state.echo = true;

    // send the init message
    protocol_startup();
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

void command_reset(const char* buffer, int paramcount)
{
    cli();
    wdt_enable(WDTO_15MS);
    while(1);
}

void command_peek(const char* buffer, int paramcount)
{
    if(buffer[0] != '0' || buffer[1] != 'x')
    {
        static const char error[] PROGMEM = 
            "usage: 0xDD or 0xdd\n";
        serial_put_P((const unsigned char*)error, sizeof(error));
        return;
    }
    static const char message1[] PROGMEM = 
        "Taking control of bus...\n";
    serial_put_P((const unsigned char*)message1, sizeof(message1));
    memory_takebus();

    unsigned int address = 0;
    for(int i=2; i < paramcount; ++i)
    {
        if(buffer[i] >= '0' && buffer[i] <= '9')
        {
            address = address << 4;
            address += buffer[i] - '0';
        }
        else if(buffer[i] >= 'A' && buffer[i] <= 'F')
        {
            address = address << 4;
            address += buffer[i] - 'A';
            address += 10;
        }
        else if(buffer[i] >= 'a' && buffer[i] <= 'f')
        {
            address = address << 4;
            address += buffer[i] - 'a';
            address += 10;
        }
    }

    char pbuffer[64];
    sprintf(pbuffer, "Reading 0x%x...",address);
    serial_put((unsigned char*)pbuffer, strlen(pbuffer));

    unsigned char result = memory_read(address);

    sprintf(pbuffer, "  Result was 0x%hhx\n",result);
    serial_put((unsigned char*)pbuffer, strlen(pbuffer));

    memory_releasebus();
}

void command_poke(const char* buffer, int paramcount)
{
    if(buffer[0] != '0' || buffer[1] != 'x')
    {
        static const char error[] PROGMEM = 
            "usage: 0xDD or 0xdd\n";
        serial_put_P((const unsigned char*)error, sizeof(error));
        return;
    }
    static const char message1[] PROGMEM = 
        "Taking control of bus...\n";
    serial_put_P((const unsigned char*)message1, sizeof(message1));
    memory_takebus();

    unsigned int address = 0;
    unsigned char data = 0;
    int phase = 1;
    for(int i=2; i < paramcount; ++i)
    {
        if(phase == 1)
        {
            if(buffer[i] >= '0' && buffer[i] <= '9')
            {
                address = address << 4;
                address += buffer[i] - '0';
            }
            else if(buffer[i] >= 'A' && buffer[i] <= 'F')
            {
                address = address << 4;
                address += buffer[i] - 'A';
                address += 10;
            }
            else if(buffer[i] >= 'a' && buffer[i] <= 'f')
            {
                address = address << 4;
                address += buffer[i] - 'a';
                address += 10;
            }
            else if(buffer[i] == ' ')
            {
                ++phase;
                i += 2; // skip 0x
            }
        }
        else
        {
            if(buffer[i] >= '0' && buffer[i] <= '9')
            {
                data = data << 4;
                data += buffer[i] - '0';
            }
            else if(buffer[i] >= 'A' && buffer[i] <= 'F')
            {
                data = data << 4;
                data += buffer[i] - 'A';
                data += 10;
            }
            else if(buffer[i] >= 'a' && buffer[i] <= 'f')
            {
                data = data << 4;
                data += buffer[i] - 'a';
                data += 10;
            }
        }
    }

    char pbuffer[64];
    sprintf(pbuffer, "writing 0x%x -> 0x%hhx...",address, data);
    serial_put((unsigned char*)pbuffer, strlen(pbuffer));

    memory_write(address, data);

    unsigned char result = memory_read(address);

    sprintf(pbuffer, "  Result was 0x%hhx\n",result);
    serial_put((unsigned char*)pbuffer, strlen(pbuffer));

    memory_releasebus();
}
