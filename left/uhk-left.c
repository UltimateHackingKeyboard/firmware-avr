#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include "../USART.h"
#include "../KeyMatrix.h"

KeyMatrix_t KeyMatrixLeft;

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

const __flash KeyMatrixInfo_t KeyMatrixInfoLeft = {
    .ColNum = LEFT_COLS_NUM,
    .RowNum = ROWS_NUM,
    .RowPins = (Pin_t[]) {
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB0 },
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB1 },
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB2 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC1 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC2 },
    },
    .ColPorts = (Pin_t[]) {
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD4 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD3 },
        // Column 3 is controlled by DRAIN7 of the TPIC6C595 power shift register.
        { .Direction=0,     .Name=0,      .Number=0 },
        { .Direction=&DDRC, .Name=&PORTC, .Number=PORTC0 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD7 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD6 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD5 },
    },
    .ShouldSetDDR = true
};

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


    KeyMatrix_Init(&KeyMatrixLeft, &KeyMatrixInfoLeft);

    while (1) {
        KeyMatrix_Scan(&KeyMatrixLeft, EnableColumn);
        for (uint8_t Row=0; Row<ROWS_NUM; Row++) {
            for (uint8_t Col=0; Col<LEFT_COLS_NUM; Col++) {
                uint8_t state = KeyMatrix_GetElement(&KeyMatrixLeft, Row, Col);
                if (IS_KEY_STATE_CHANGED(state)) {
                    uint8_t IsKeyPressed = GET_KEY_STATE_CURRENT(state);
                    uint8_t Event = EVENT_TYPE_KEY |
                                    CONSTRUCT_EVENT_STATE(IsKeyPressed) |
                                    CONSTRUCT_KEYCODE(Row, Col, LEFT_COLS_NUM);
                    USART_SendByte(Event);
                }
            }
        }
        _delay_ms(10);
    }
}
