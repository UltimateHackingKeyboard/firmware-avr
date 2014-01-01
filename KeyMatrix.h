#ifndef __KEYMATRIX_H__
#define __KEYMATRIX_H__

#include "TinyPin.h"
#include "TinyPort.h"
#include "Pin.h"
#include "Port.h"
#include "Event.h"
#include "KeyState.h"

typedef struct {
    uint8_t ColNum;
    uint8_t RowNum;
    TinyPort_t *ColPorts;
    TinyPin_t *RowPins;
    uint8_t *Matrix;
} KeyMatrix_t;

void KeyMatrix_Init(KeyMatrix_t *KeyMatrix, uint8_t RowNum, uint8_t ColNum);
uint8_t KeyMatrix_GetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col);
void KeyMatrix_SetElement(KeyMatrix_t *KeyMatrix, uint8_t Row, uint8_t Col, uint8_t Element);
void KeyMatrix_SetColumnPortsAndRowPins(KeyMatrix_t *KeyMatrix, Port_t *ColPorts, Pin_t *RowPins);
void KeyMatrix_Scan(KeyMatrix_t *KeyMatrix, uint8_t EnableColumn(uint8_t));
void KeyMatrix_ReceiveEvent(KeyMatrix_t *KeyMatrix, uint8_t Event);

#endif
