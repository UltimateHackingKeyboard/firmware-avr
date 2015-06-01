/** \file
 *
 *  Main file for the left keyboard half.
 */

#include "uhk-left.h"

KeyMatrix_t KeyMatrixLeft;
bool IsKeyMatrixShiftRegisterColEnabled = false;

uint8_t SetColCallback(uint8_t col)
{
    if (col == KEY_MATRIX_SHIFT_REGISTER_COL_ID1) {
        IsKeyMatrixShiftRegisterColEnabled = true;
        ShiftRegister_Transmit(1 << SHIFT_REGISTER_KEY_MATRIX_DRAIN_ID1);
        return true;
    } else if (col == KEY_MATRIX_SHIFT_REGISTER_COL_ID2) {
        IsKeyMatrixShiftRegisterColEnabled = true;
        ShiftRegister_Transmit(1 << SHIFT_REGISTER_KEY_MATRIX_DRAIN_ID2);
        return true;
    } else {
        IsKeyMatrixShiftRegisterColEnabled = false;
        ShiftRegister_Transmit(0);
        return false;
    }
}

const __flash KeyMatrixInfo_t KeyMatrixInfoLeft = {
    .ColNum = LEFT_COLS_NUM,
    .RowNum = ROWS_NUM,
    .RowPins = (Pin_t[]) {
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC1 },
        { .Direction=&DDRD, .Name=&PIND, .Number=PIND2 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC2 },
        { .Direction=&DDRD, .Name=&PIND, .Number=PIND4 },
        { .Direction=&DDRD, .Name=&PIND, .Number=PIND5 },
    },
    .ColPorts = (Pin_t[]) {
        { .Direction=&DDRC, .Name=&PORTC, .Number=PORTC0 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD3 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD6 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD7 },
        { .Direction=&DDRB, .Name=&PORTB, .Number=PORTB1 },
        // Column 6 is controlled by DRAIN7 of the TPIC6C595 power shift register.
        { .Direction=0,     .Name=0,      .Number=0      },
        // Column 7 is controlled by DRAIN6 of the TPIC6C595 power shift register.
        { .Direction=0,     .Name=0,      .Number=0      },
    },
    .ShouldSetDDR = true
};

uint8_t *KeyMatrixDataLeft[LEFT_COLS_NUM*ROWS_NUM];

int main(void)
{
    sei();
    USART_Init();
    TWI_Init();
    PCA9634_Init();
    LedMatrix_SetRows(0xf0);
//    LedMatrix_Init();
    ShiftRegister_Init();
    KeyMatrix_Init(&KeyMatrixLeft, &KeyMatrixInfoLeft, (uint8_t*)&KeyMatrixDataLeft);

    while (true) {
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

// USART RX interrupt receiving bytes from the right half.
ISR(USART_RX_vect, ISR_BLOCK)
{
    if (USART_ReceiveByte() == 'r') {
        cli();
        wdt_enable(WDTO_15MS);
        for (;;);
    }
}

// LED matrix interrupt PWMing the LEDs.
ISR(TIMER1_COMPA_vect)
{
    // TODO: This ISR is way too heavy.  Gotta make it much lighter weight.
    LedMatrix_UpdateNextRow(IsKeyMatrixShiftRegisterColEnabled);
}
