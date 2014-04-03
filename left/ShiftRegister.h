#ifndef __SHIFT_REGISTER_H__
#define __SHIFT_REGISTER_H__

    /* Includes: */
        #include <avr/io.h>

    /* Macros: */
        #define DDR_MOSI DDRB
        #define DDB_MOSI DDB3
        #define DDR_OE   DDRB
        #define DDB_OE   DDB2
        #define DDR_SCK  DDRB
        #define DDB_SCK  DDB5
        #define DDR_RCK  DDRC
        #define DDB_RCK  DDC3

        #define PORT_OE  PORTB
        #define PD_OE    PB2
        #define PORT_RCK PORTC
        #define PD_RCK   PC3

    /* Function prototypes: */
        void ShiftRegister_Init();
        void ShiftRegister_Transmit(uint8_t data);

#endif
