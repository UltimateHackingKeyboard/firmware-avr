#include <avr/io.h>
#include <util/delay.h>
#include "../USART.c"
#include "../KeyMatrix.c"
#include "../keycode.h"

#define ROW_NUM 5
#define COL_NUM 7

KeyMatrix_t keyMatrix;

char SpiTransmit(char data)
{
    SPDR = data;

    // TODO: Investigate on why the following loop blocks and uncomment it eventually.
    // while (!(SPSR & (1<<SPIF))) {
    // }

    // Copy data to the storage register by toggling the latch.
    PORTC |= (1<<PC3);
    PORTC &= ~(1<<PC3);

    return SPDR;
}

uint8_t EnableColumn(uint8_t col)
{
    if (col == 2) {
        SpiTransmit(0xff);
        return 1;
    } else {
        SpiTransmit(0x00);
        return 0;
    }
}

int main(void)
{
    USART_Init();

    /* Initialize SPI */

    DDRB |= 1<<DDB3;  // MOSI is output.
    DDRD |= 1<<DDD2;  // OE is output.
    DDRB |= 1<<DDB5;  // SCK is output.
    DDRC |= 1<<DDC3;  // RCK is output.
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<CPOL) | (1<<CPHA);  // SPI enable, Master, f/16
    SpiTransmit(0);
    PORTD &= ~(1<<PD2);  // Enable OE by pulling it low.

    Port_t column_ports[COL_NUM] = {
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD4 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD3 },
        // Column 3 is controlled by DRAIN7 of the TPIC6C595 power shift register.
        { .Direction=0,     .Name=0,      .Number=0 },
        { .Direction=&DDRC, .Name=&PORTC, .Number=PORTC0 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD7 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD6 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD5 },
    };

    Pin_t row_pins[ROW_NUM] = {
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB0 },
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB1 },
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB2 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC1 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC2 },
    };

    KeyMatrix_Init(&keyMatrix, ROW_NUM, COL_NUM);
    KeyMatrix_SetColPortsAndRowPins(&keyMatrix, column_ports, row_pins);

    while (1) {
        KeyMatrix_Scan(&keyMatrix, EnableColumn);
        for (uint8_t row=0; row<ROW_NUM; row++) {
            for (uint8_t col=0; col<COL_NUM; col++) {
                uint8_t state = KeyMatrix_GetElement(&keyMatrix, row, col);
                if (IS_KEY_STATE_CHANGED(state)) {
                    uint8_t is_key_pressed = GET_KEY_STATE_CURRENT(state);
                    uint8_t event = EVENT_TYPE_KEY |
                                    CONSTRUCT_EVENT_STATE(is_key_pressed) |
                                    CONSTRUCT_KEYCODE(row, col, COL_NUM);
                    USART_SendByte(event);
                }
            }
        }
        _delay_ms(10);
    }
}
