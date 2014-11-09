#ifndef __LED_MATRIX_H__
#define __LED_MATRIX_H__

    /* Includes: */
        #include <avr/io.h>
        #include <avr/interrupt.h>
        #include <stdbool.h>
        #include "ShiftRegister.h"
        #include "PCA9634.h"

    /* Macros: */
        #define LED_MATRIX_ROWS_NUM 8

    /* Function prototypes: */
        void LedMatrix_Init(void);
        void LedMatrix_SetRows(uint8_t EnabledRowsBitmask);
		void LedMatrix_UpdateNextRow(bool IsKeyboardColEnabled);

#endif
