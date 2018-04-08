/******************************************************************************
 * z80-board-firmware
 * i2c.c - i2c functionality
 *
 *****************************************************************************/

#include "i2c.h"
#include "device.h"

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
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    // wait twint
    while (!(TWCR & (1<<TWINT)));
    // check twi status
    return ((TWSR & 0xF8) == 0x08) || ((TWSR & 0xF8) == 0x10);
}

static void i2c_stop()
{
    TWCR = (1<<TWINT)| (1<<TWEN) | (1<<TWSTO);
    while(TWCR & (1 << TWSTO));
}

static bool i2c_address(uint8_t address, bool read)
{
    TWDR = address << 1 | (read ? 1 : 0);
    TWCR = (1<<TWINT) | (1<<TWEN);
    // wait twint
    while (!(TWCR & (1<<TWINT)));
    // check twi status
    return ((TWSR & 0xF8) == 0x18) || ((TWSR & 0xF8) == 0x40);
}

static bool i2c_senddata(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    // wait twint
    while (!(TWCR & (1<<TWINT)));
    return ((TWSR & 0xF8) == 0x28);
}

static bool i2c_recvdata(uint8_t* data)
{
    TWCR = (1<<TWINT) | (1 << TWEA) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if((TWSR & 0xF8) == 0x50)
    {
        *data = TWDR;
        return true;
    }
   
    return false;
}

static int i2c_readinternal(uint8_t* buffer, uint8_t length)
{
    uint8_t counter = 0;

    while(length)
    {
        if(!i2c_recvdata(buffer))
        {
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
    if(!i2c_address(address, true)) { i2c_stop(); return 0; }
    
    return i2c_readinternal(buffer, length);
}

void i2c_write(uint8_t address, uint8_t* data, uint8_t length)
{
    if(!i2c_start()) { return; }
    //send address + W
    if(!i2c_address(address, false)) { i2c_stop(); return; }
    // send data
    while(length)
    {
        if(!i2c_senddata(data[0])) { i2c_stop(); return; }
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
    if(!i2c_address(address, false)) { i2c_stop(); return 0; }
    // send byte
    if(!i2c_senddata(data)) { i2c_stop(); return 0; }
    // restart in read mode
    i2c_start();
    if(!i2c_address(address, true)) { i2c_stop(); return 0; }
    // now receive
    return i2c_readinternal(buffer, length);
}

uint8_t i2c_readbyte(uint8_t address)
{
    if(!i2c_start()) { return 0; }
    if(!i2c_address(address, true)) { i2c_stop(); return 0; }
    uint8_t b;
    i2c_readinternal(&b, 1);
    return b;
    if(i2c_recvdata(&b))
    {
        i2c_stop();
        return b;
    }

   
    i2c_stop();
    return 0;
}