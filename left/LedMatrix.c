#include "LedMatrix.h"

//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0b10011111, 0b11000110, 0b00110101, 0b11110000, 0b01111100, 0b11001100};
//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {1, 2, 4, 8, 16, 32};
//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0xff, 0, 0, 0, 0, 0};
//uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f};
uint8_t LedStates[LED_MATRIX_ROWS_NUM] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t ActiveLedMatrixCol = 0;

// Initialize timer1 to PWM the LEDs.
void LedMatrix_Init()
{
    cli();                    // Disable all interrupts.
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    OCR1A = 10;               // compare match register 16MHz/256/2Hz
    TCCR1B |= (1 << WGM12);   // CTC mode
    TCCR1B |= (1 << CS12);    // 256 prescaler
    TIMSK1 |= (1 << OCIE1A);  // Enable timer compare interrupt.
    sei();                    // Enable all interrupts.
}

void LedMatrix_SetRows(uint8_t ColsBitmask)
{
    TWI_Start();
    TWI_Write(PCA9634_ADDRESS);
    TWI_Write(PCA9634_AI_PWM | PCA9634_REG_PWM0);

    for (uint8_t Col=0; Col<LED_MATRIX_COLS_NUM; Col++) {
        uint8_t IsColSet = ColsBitmask & (1<<Col);
        TWI_Write(IsColSet ? 0 : 0xff);
    }

    TWI_Stop();
}

void LedMatrix_UpdateNextRow()
{
    // TODO: Figure out whether the rows or the columns should be iterated to achieve
    // maximum brightness.

//    ShiftRegister_Transmit(LedStates[ActiveLedMatrixCol]);
//    LedMatrix_SetRows(1<<ActiveLedMatrixCol);

    // The switch register seems to switch slowly so the LED matrix must be cleared
    // before the update or else a second set of dim segments will light up.
    LedMatrix_SetRows(0);
    ShiftRegister_Transmit(1<<ActiveLedMatrixCol);
    LedMatrix_SetRows(LedStates[ActiveLedMatrixCol]);

    if (++ActiveLedMatrixCol == LED_MATRIX_ROWS_NUM) {
          ActiveLedMatrixCol = 0;
    }
}
