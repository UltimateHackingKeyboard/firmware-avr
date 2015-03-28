#ifndef __EEPROM_H__
#define __EEPROM_H__

    /* Includes: */
        #include <avr/io.h>

    /* Macros: */
        #define EEPROM_WRITE_ADDRESS 0b10100000
        #define EEPROM_READ_ADDRESS  0b10100001

    /* Function Prototypes: */
        void EEPROM_Read(uint8_t* Destination, uint8_t Length);
        void EEPROM_Write(uint8_t* Source, uint8_t Length);

#endif
