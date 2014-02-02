#ifndef __KEY_MATRIX_H__
#define __KEY_MATRIX_H__

    /* Includes: */
        #include <avr/io.h>
        #include <stdlib.h>
        #include <avr/pgmspace.h>
        #include "Event.h"
        #include "KeyState.h"
        #include "KeyCode.h"

    /* Macros: */
        #define LEFT_COLS_NUM 7
        #define RIGHT_COLS_NUM 7
        #define TOTAL_COLS_NUM (LEFT_COLS_NUM + RIGHT_COLS_NUM)
        #define ROWS_NUM 5

    /* Type Defines: */
        typedef struct {
            // TODO: Investigate on why these volatile qualifiers are needed to avoid the following compiler warning:
            // warning: initialization discards ‘volatile’ qualifier from pointer target type [enabled by default]
            volatile uint8_t *Direction;
            volatile uint8_t *Name;
            uint8_t Number;
        } Pin_t;

        /* This part of the key matrix is to be stored in the Flash to save SRAM space. */
        typedef struct {
            const uint8_t ShouldSetDDR;
            const uint8_t ColNum;
            const uint8_t RowNum;
            const Pin_t *ColPorts;
            const Pin_t *RowPins;
        } KeyMatrixInfo_t;

        /* This Part of the key matrix is to be stored in the SRAM. */
        typedef struct {
            const __flash KeyMatrixInfo_t *Info;
            uint8_t *Matrix;
        } KeyMatrix_t;

    /* Function Prototypes: */
        void KeyMatrix_Init(KeyMatrix_t *KeyMatrix, const __flash KeyMatrixInfo_t *KeyMatrixInfo);
        uint8_t KeyMatrix_GetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col);
        void KeyMatrix_SetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col, uint8_t Element);

        /* The optional SetColCallback is meant to enable/disable columns when scanning the keyboard matrix.
         * It is needed when some columns are not directly wired to GPIO pins but to other parts like shift registers.
         * If the callback is unused then simply pass NULL.
         * If the callback is used then return true for callback-handled non-GPIO pins and
         * false for unhandled GPIO pins that will be enabled/disabled by KeyMatrix_Scan()
         */
        void KeyMatrix_Scan(KeyMatrix_t *KeyMatrix, uint8_t SetColCallback(uint8_t));

#endif
