#include "KeyMatrix.h"

void KeyMatrix_Init(KeyMatrix_t *KeyMatrix, const __flash KeyMatrixInfo_t *KeyMatrixInfo)
{
    uint8_t RowNum = KeyMatrixInfo->RowNum;
    uint8_t ColNum = KeyMatrixInfo->ColNum;

    KeyMatrix->Info = KeyMatrixInfo;
    KeyMatrix->Matrix = calloc(RowNum*ColNum, sizeof(uint8_t));

    if (!KeyMatrixInfo->ShouldSetDDR) {
        return;
    }

    for (uint8_t Col=0; Col<ColNum; Col++) {
        const Pin_t *ColPort = KeyMatrixInfo->ColPorts + Col;
        *ColPort->Direction |= 1 << ColPort->Number;  // Set DDR bit as output.
    }

    for (uint8_t Row=0; Row<RowNum; Row++) {
        const Pin_t *RowPin = KeyMatrixInfo->RowPins + Row;
        *RowPin->Direction &= ~(1 << RowPin->Number);  // Set DDR bit as input.
    }
}

uint8_t KeyMatrix_GetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col)
{
    return KeyMatrix->Matrix[Col*KeyMatrix->Info->RowNum + Row];
}

void KeyMatrix_SetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col, uint8_t Element)
{
    KeyMatrix->Matrix[Col*KeyMatrix->Info->RowNum + Row] = Element;
}

void KeyMatrix_Scan(KeyMatrix_t *KeyMatrix, uint8_t SetColCallback(uint8_t))
{
    for (uint8_t Col=0; Col<KeyMatrix->Info->ColNum; Col++) {
        uint8_t ColumnAlreadyEnabled = SetColCallback == NULL ? 0 : SetColCallback(Col);
        const Pin_t *ColPort = KeyMatrix->Info->ColPorts + Col;

        if (!ColumnAlreadyEnabled) {
            // Enable the output port of the column.
            *ColPort->Name |= 1<<ColPort->Number;
        }

        for (uint8_t Row=0; Row<KeyMatrix->Info->RowNum; Row++) {
            const Pin_t *RowPin = KeyMatrix->Info->RowPins + Row;
            // Read the input pin of the row.
            uint8_t IsKeyPressed = *RowPin->Name & 1<<RowPin->Number;
            uint8_t WasKeyPressed = GET_KEY_STATE_PREV(KeyMatrix_GetElement(KeyMatrix, Row, Col));
            uint8_t KeyState = CONSTRUCT_KEY_STATE(WasKeyPressed, IsKeyPressed);
            KeyMatrix_SetElement(KeyMatrix, Row, Col, KeyState);
        }

        if (!ColumnAlreadyEnabled) {
            // Disable the output port of the column.
            *(ColPort->Name) &= ~(1<<ColPort->Number);
        }
    }
}
