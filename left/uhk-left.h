#ifndef _UHK_LEFT_H_
#define _UHK_LEFT_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/pgmspace.h>
        #include <avr/interrupt.h>
        #include <avr/wdt.h>
        #include <util/delay.h>
        #include <stdbool.h>
        #include "KeyMatrix.h"
        #include "USART.h"

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

#endif
