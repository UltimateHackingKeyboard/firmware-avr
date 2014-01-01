#include <stdlib.h>
#include "KeyMatrix.h"

void KeyMatrix_Init(KeyMatrix_t *KeyMatrix, uint8_t RowNum, uint8_t ColNum)
{
    KeyMatrix->RowNum = RowNum;
    KeyMatrix->ColNum = ColNum;
    KeyMatrix->Matrix = calloc(RowNum * ColNum, sizeof(uint8_t));
}

uint8_t KeyMatrix_GetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col)
{
    return KeyMatrix->Matrix[Col*KeyMatrix->RowNum + Row];
}

void KeyMatrix_SetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col, uint8_t Element)
{
    KeyMatrix->Matrix[Col*KeyMatrix->RowNum + Row] = Element;
}

void KeyMatrix_SetColPortsAndRowPins(KeyMatrix_t *KeyMatrix, Port_t *ColPorts, Pin_t *RowPins)
{
    KeyMatrix->ColPorts = malloc(KeyMatrix->ColNum * sizeof(TinyPort_t));
    for (uint8_t col=0; col<KeyMatrix->ColNum; col++) {
        Port_t *colPort = ColPorts + col;
        *colPort->Direction |= 1 << (colPort->Number);  // Set DDR bit as output.
        TinyPort_t *tinyPort = KeyMatrix->ColPorts + col;
        tinyPort->Name = colPort->Name;
        tinyPort->Number = colPort->Number;
    }

    KeyMatrix->RowPins = malloc(KeyMatrix->RowNum * sizeof(TinyPin_t));
    for (uint8_t row=0; row<KeyMatrix->RowNum; row++) {
        Pin_t *rowPin = RowPins + row;
        *rowPin->Direction &= ~(1 << (rowPin->Number));  // Set DDR bit as input.
        TinyPin_t *tinyPin = KeyMatrix->RowPins + row;
        tinyPin->Name = rowPin->Name;
        tinyPin->Number = rowPin->Number;
    }
}

void KeyMatrix_Scan(KeyMatrix_t *KeyMatrix, uint8_t EnableColumn(uint8_t))
{
    for (uint8_t col=0; col<KeyMatrix->ColNum; col++) {
        // Enable the output port of the column
        uint8_t ColumnAlreadyEnabled = EnableColumn == NULL ? 0 : EnableColumn(col);
        TinyPort_t *ColPort = KeyMatrix->ColPorts + col;
        if (!ColumnAlreadyEnabled) {
            *(ColPort->Name) |= 1<<ColPort->Number;
        }
        for (uint8_t row=0; row<KeyMatrix->RowNum; row++) {
            TinyPin_t *RowPin = KeyMatrix->RowPins + row;
            // Read the input pin of the row
            uint8_t isKeyPressed = *RowPin->Name & 1<<RowPin->Number;
            uint8_t wasKeyPressed = GET_KEY_STATE_PREV(KeyMatrix_GetElement(KeyMatrix, row, col));
            uint8_t keyState = CONSTRUCT_KEY_STATE(wasKeyPressed, isKeyPressed);
            KeyMatrix_SetElement(KeyMatrix, row, col, keyState);
        }
        if (!ColumnAlreadyEnabled) {
            *(ColPort->Name) &= ~(1<<ColPort->Number);
        }
    }
}
