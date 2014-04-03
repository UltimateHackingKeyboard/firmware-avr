/** \file
 *
 *  Main file for the left keyboard half.
 */

#include "uhk-left.h"

KeyMatrix_t KeyMatrixLeft;

#define LED_MATRIX_ROWS_NUM 8
//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0b10011111, 0b11000110, 0b00110101, 0b11110000, 0b01111100, 0b11001100};
//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0, 0, 0, 0, 0, 0};
uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f};
uint8_t ActiveLedMatrixRow = 0;
uint8_t IsKeyboardColEnabled = 0;

// Initialize timer1 to PWM the LEDs.
void LedMatrix_Init()
{
  cli();                    // Disable all interrupts.
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 50;               // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // Enable timer compare interrupt.
  sei();                    // Enable all interrupts.
}

void LedMatrix_EnableRows(uint8_t EnabledRowsBitmask)
{
  TWI_Start();
  TWI_Write(PCA9634_ADDRESS);
  TWI_Write(PCA9634_AI_PWM | PCA9634_REG_PWM0);

  for (uint8_t Row=0; Row<LED_MATRIX_ROWS_NUM; Row++) {
    uint8_t IsRowEnabled = EnabledRowsBitmask & (1<<Row);
    TWI_Write(IsRowEnabled ? 0 : 0xff);
  }

  TWI_Stop();
}

uint8_t SetColCallback(uint8_t col)
{
    if (col == 2) {
        IsKeyboardColEnabled = 1;
        ShiftRegister_Transmit(1<<7/* | 1<<0*/);
        return true;
    } else {
        IsKeyboardColEnabled = 0;
        ShiftRegister_Transmit(0x00);
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
    TWI_Init();
    PCA9634_Init();
    LedMatrix_EnableRows(0xf0);
    LedMatrix_Init();
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

// USART RX interrupt to receive bytes from the right half.
ISR(USART_RX_vect, ISR_BLOCK)
{
    if (USART_ReceiveByte() == 'r') {
        cli();
        wdt_enable(WDTO_15MS);
        for (;;);
    }
}

// LED matrix interrupt to PWM the LEDs.
ISR(TIMER1_COMPA_vect)
{
    // TODO: This interrupt routine is way too heavy.  Gotta make it much lighter weight.
    ShiftRegister_Transmit(LedStates[ActiveLedMatrixRow] | IsKeyboardColEnabled<<7);
    LedMatrix_EnableRows(1<<ActiveLedMatrixRow);

    if (++ActiveLedMatrixRow == LED_MATRIX_ROWS_NUM) {
          ActiveLedMatrixRow = 0;
    }
}
