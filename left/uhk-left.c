/** \file
 *
 *  Main file for the left keyboard half.
 */

#include "uhk-left.h"

KeyMatrix_t KeyMatrixLeft;

void SpiTransmit(char data)
{
    SPDR = data;
    while (!(SPSR & (1<<SPIF)));

    // Copy data to the storage register by toggling the latch.
    PORT_RCK |= (1<<PD_RCK);
    PORT_RCK &= ~(1<<PD_RCK);
}

uint8_t SetColCallback(uint8_t col)
{
    if (col == 2) {
        SpiTransmit(1<<7);
        return true;
    } else {
        SpiTransmit(0x00);
        return false;
    }
}

const __flash KeyMatrixInfo_t KeyMatrixInfoLeft = {
    .ColNum = LEFT_COLS_NUM,
    .RowNum = ROWS_NUM,
    .RowPins = (Pin_t[]) {
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB0 },
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB1 },
        { .Direction=&DDRD, .Name=&PIND, .Number=PIND2 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC1 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC2 },
    },
    .ColPorts = (Pin_t[]) {
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD4 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD3 },
        // Column 3 is controlled by DRAIN7 of the TPIC6C595 power shift register.
        { .Direction=0,     .Name=0,      .Number=0      },
        { .Direction=&DDRC, .Name=&PORTC, .Number=PORTC0 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD7 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD6 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD5 },
    },
    .ShouldSetDDR = true
};

uint8_t *KeyMatrixDataLeft[LEFT_COLS_NUM*ROWS_NUM];

int main(void)
{
    sei();
    USART_Init();

    /* Initialize SPI */
    DDR_MOSI |= 1<<DDB_MOSI; // MOSI is output.
    DDR_OE   |= 1<<DDB_OE;   // OE is output.
    DDR_SCK  |= 1<<DDB_SCK;  // SCK is output.
    DDR_RCK  |= 1<<DDB_RCK;  // RCK is output.
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1);  // SPI enable, Master, f/16
    SpiTransmit(0);
    PORT_OE &= ~(1<<PD_OE);  // Enable OE by pulling it low.

    KeyMatrix_Init(&KeyMatrixLeft, &KeyMatrixInfoLeft, (uint8_t*)&KeyMatrixDataLeft);

    while (1) {
        KeyMatrix_Scan(&KeyMatrixLeft, SetColCallback);
        for (uint8_t Row=0; Row<ROWS_NUM; Row++) {
            for (uint8_t Col=0; Col<LEFT_COLS_NUM; Col++) {
                uint8_t KeyState = KeyMatrix_GetElement(&KeyMatrixLeft, Row, Col);
                if (KEY_STATE_CHANGED(KeyState)) {
                    uint8_t IsKeyPressed = KEY_STATE_IS_PRESSED(KeyState);
                    uint8_t Event = CONSTRUCT_EVENT_STATE(IsKeyPressed) |
                                    CONSTRUCT_KEYCODE(Row, Col, LEFT_COLS_NUM);
                    USART_SendByte(Event);
                }
            }
        }
        // TODO: Implement proper debouncing algorithm that does not block.
        _delay_ms(10);
    }
}

ISR(USART_RX_vect, ISR_BLOCK)
{
    if (USART_ReceiveByte() == 'r') {
        cli();
        wdt_enable(WDTO_15MS);
        for (;;);
    }
}
