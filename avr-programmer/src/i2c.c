/******************************************************************************
 * z80-board-firmware
 * i2c.c - i2c functionality
 *
 *****************************************************************************/

#include "i2c.h"
#include "device.h"
#include "debug.h"

void i2c_init()
{
    // enable pullups on I2C pins
    I2C_PORT |= (1 << I2C_PIN_SDA) | (1 << I2C_PIN_SCL);

    // set bitrate scaler
    TWBR = 2;
    TWSR = 2; // prescaler 4

    // enable TWI
    TWCR = (1 << TWEN);
}

// returns true on success
static bool i2c_start()
{
    EXTREME_DEBUG_LOG_STRING("Sending Start");
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    // wait twint
    while (!(TWCR & (1<<TWINT)));
    // check twi status
    EXTREME_DEBUG_LOG_STRING("Sent Start");
    return ((TWSR & 0xF8) == 0x08) || ((TWSR & 0xF8) == 0x10);
}

static void i2c_stop()
{
    EXTREME_DEBUG_LOG_STRING("Sending Stop");
    DEBUG_LOG_VAL("Status Register", (TWSR & 0xF8));
    TWCR = (1<<TWINT)| (1<<TWEN) | (1<<TWSTO);
    while(TWCR & (1 << TWSTO));
    EXTREME_DEBUG_LOG_STRING("Sent Stop");
}

static bool i2c_address(uint8_t address, bool read)
{
    EXTREME_DEBUG_LOG_STRING("Sending Address");
    TWDR = address << 1 | (read ? 1 : 0);
    TWCR = (1<<TWINT) | (1<<TWEN);
    // wait twint
    while (!(TWCR & (1<<TWINT)));
    // check twi status
    EXTREME_DEBUG_LOG_STRING("Sent Address");
    return ((TWSR & 0xF8) == 0x18) || ((TWSR & 0xF8) == 0x40);
}

static bool i2c_senddata(uint8_t data)
{
    EXTREME_DEBUG_LOG_VAL("Sending Data", data);
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    // wait twint
    while (!(TWCR & (1<<TWINT)));
    EXTREME_DEBUG_LOG_STRING("Sent Data");
    return ((TWSR & 0xF8) == 0x28);
}

static bool i2c_recvdata(uint8_t* data, bool last)
{
    EXTREME_DEBUG_LOG_STRING("Receiving Data");
    if(last)
    {
        EXTREME_DEBUG_LOG_STRING("This is the last expected byte");
    }
    TWCR = (1<<TWINT) | (last ? 0 : (1 << TWEA)) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if((!last && (TWSR & 0xF8) == 0x50) ||
        (last && (TWSR & 0xF8) == 0x58))
    {
        *data = TWDR;
        EXTREME_DEBUG_LOG_STRING("Received Data");
        return true;
    }
    EXTREME_DEBUG_LOG_STRING("Did not receive Data");
    return false;
}

static int i2c_readinternal(uint8_t* buffer, uint8_t length)
{
    uint8_t counter = 0;

    while(length)
    {
        DEBUG_LOG_VAL("Receiving remaining", length);
        if(!i2c_recvdata(buffer, length == 1))
        {
            DEBUG_LOG_VAL("Error receiving", (TWSR & 0xF8));
            i2c_stop();
            return counter;
        }

        --length;
        ++counter;
        ++buffer;
    }

    i2c_stop();

    return counter;
}

int i2c_read(uint8_t address, uint8_t* buffer, uint8_t length)
{
    if(!i2c_start()) { return 0; }
    if(!i2c_address(address, true))
    {
        DEBUG_LOG_VAL("Error addressing", (TWSR & 0xF8));
        i2c_stop();
        return 0;
    }
    
    return i2c_readinternal(buffer, length);
}

void i2c_write(uint8_t address, uint8_t* data, uint8_t length)
{
    if(!i2c_start()) { return; }
    //send address + W
    if(!i2c_address(address, false))
    {
        DEBUG_LOG_VAL("Error addressing", (TWSR & 0xF8));
        i2c_stop();
        return;
    }
    // send data
    while(length)
    {
        if(!i2c_senddata(data[0]))
        {
            DEBUG_LOG_VAL("Error sending", (TWSR & 0xF8));
            i2c_stop(); return;
        }
        --length;
        ++data;
    }
    // send stop
    i2c_stop();
}

int i2c_writeread(uint8_t address, uint8_t data, uint8_t* buffer, uint8_t length)
{
    if(!i2c_start()) { return 0; }
    //send address + W
    if(!i2c_address(address, false))
    {
        DEBUG_LOG_VAL("Error addressing", (TWSR & 0xF8));
        i2c_stop();
        return 0;
    }
    // send byte
    if(!i2c_senddata(data))
    {
        DEBUG_LOG_VAL("Error sending", (TWSR & 0xF8));
        i2c_stop();
        return 0;
    }
    // restart in read mode
    i2c_start();
    if(!i2c_address(address, true))
    {
        DEBUG_LOG_VAL("Error addressing", (TWSR & 0xF8));
        i2c_stop();
        return 0;
    }
    // now receive
    return i2c_readinternal(buffer, length);
}

uint8_t i2c_readbyte(uint8_t address)
{
    if(!i2c_start())
    {
        DEBUG_LOG_VAL("Error starting", (TWSR & 0xF8));
        return 0;
    }

    if(!i2c_address(address, true))
    {
        DEBUG_LOG_VAL("Error addressing", (TWSR & 0xF8));
        i2c_stop();
        return 0;
    }
    uint8_t b;
    i2c_readinternal(&b, 1);
    return b;
    if(i2c_recvdata(&b, true))
    {
        DEBUG_LOG_VAL("Error reading", (TWSR & 0xF8));
        i2c_stop();
        return b;
    }

   
    i2c_stop();
    return 0;
}
