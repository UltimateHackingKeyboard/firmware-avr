// Library taken from http://www.embedds.com/programming-avr-i2c-interface/
// Respective licence applies.

#ifndef __TWI_H__
#define __TWI_H__

    /* Includes: */
        #include <avr/io.h>

    /* Function prototypes: */
        void TWI_Init(void);
        void TWI_Start(void);
        void TWI_Stop(void);
        void TWI_Write(uint8_t data);
        uint8_t TWI_ReadACK(void);
        uint8_t TWI_ReadNACK(void);
        uint8_t TWI_GetStatus(void);

#endif
