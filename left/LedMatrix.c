#include "LedMatrix.h"

//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0b10011111, 0b11000110, 0b00110101, 0b11110000, 0b01111100, 0b11001100};
//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0, 0, 0, 0, 0, 0};
uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f};
uint8_t ActiveLedMatrixRow = 0;

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

void LedMatrix_SetRows(uint8_t RowsBitmask)
{
    TWI_Start();
    TWI_Write(PCA9634_ADDRESS);
    TWI_Write(PCA9634_AI_PWM | PCA9634_REG_PWM0);

    for (uint8_t Row=0; Row<LED_MATRIX_ROWS_NUM; Row++) {
        uint8_t IsRowSet = RowsBitmask & (1<<Row);
        TWI_Write(IsRowSet ? 0 : 0xff);
    }

    TWI_Stop();
}

void LedMatrix_UpdateNextRow(bool IsKeyboardColEnabled)
{
    ShiftRegister_Transmit(LedStates[ActiveLedMatrixRow] | IsKeyboardColEnabled<<7);
    LedMatrix_SetRows(1<<ActiveLedMatrixRow);

    if (++ActiveLedMatrixRow == LED_MATRIX_ROWS_NUM) {
          ActiveLedMatrixRow = 0;
    }
}
