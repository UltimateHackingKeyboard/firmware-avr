#include "EEPROM.h"

void EEPROM_Write(uint8_t* Source, uint8_t Length) {
    TWI_Start();
    TWI_Write(EEPROM_WRITE_ADDRESS);
    TWI_Write(0);
    TWI_Write(0);

    for (uint8_t i=0; i<Length; i++) {
        TWI_Write(Source[i]);
    }

    TWI_Stop();
}

void EEPROM_Read(uint8_t* Destination, uint8_t Length) {
    TWI_Start();
    TWI_Write(EEPROM_WRITE_ADDRESS);
    TWI_Write(0);
    TWI_Write(0);

    TWI_Start();
    TWI_Write(EEPROM_READ_ADDRESS);

    for (uint8_t i=0; i<Length; i++) {
        Destination[i] = i==Length-1 ? TWI_ReadNACK() : TWI_ReadACK();
    }

    TWI_Stop();
}
