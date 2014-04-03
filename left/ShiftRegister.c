#include "ShiftRegister.h"

void ShiftRegister_Init()
{
    DDR_MOSI |= 1<<DDB_MOSI; // MOSI is output.
    DDR_OE   |= 1<<DDB_OE;   // OE is output.
    DDR_SCK  |= 1<<DDB_SCK;  // SCK is output.
    DDR_RCK  |= 1<<DDB_RCK;  // RCK is output.
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1);  // SPI enable, Master, f/16
    ShiftRegister_Transmit(0);
    PORT_OE &= ~(1<<PD_OE);  // Enable OE by pulling it low.
}

void ShiftRegister_Transmit(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1<<SPIF)));

    // Copy data to the storage register by toggling the latch.
    PORT_RCK |= (1<<PD_RCK);
    PORT_RCK &= ~(1<<PD_RCK);
}
