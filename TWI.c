// Library taken from http://www.embedds.com/programming-avr-i2c-interface/
// Respective licence applies.

#include "TWI.h"

// Set SCL to 400kHz and enable TWI.
void TWI_Init(void)
{
    TWSR = 0x00;
    TWBR = 0x0C;
    TWCR = (1<<TWEN);
}

void TWI_Start(void)
{
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

void TWI_Stop(void)
{
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void TWI_Write(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t TWI_ReadACK(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

// Read byte with NACK.
uint8_t TWI_ReadNACK(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

uint8_t TWI_GetStatus(void)
{
    uint8_t status = TWSR & 0xF8;  // Mask status.
    return status;
}
