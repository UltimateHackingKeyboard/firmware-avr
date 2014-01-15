/** \file
 *
 *  HID report handlers for the keyboard.
 */

#include "KeyboardReports.h"

bool CreateKeyboardHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    _delay_ms(10);  // Work around key bouncing.

    USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
    uint8_t UsedKeyCodes = 0;

/*
    // Update the left keyboard matrix.
    while (USART_HasByte()) {
        uint8_t event = USART_ReceiveByte();
        uint8_t keyState = GET_EVENT_STATE(event);
        uint8_t keyId = GET_EVENT_PAYLOAD(event);
        uint8_t row = keyId / leftMatrix->ColNum;
        uint8_t col = keyId % leftMatrix->ColNum;
        KeyMatrix_SetElement(leftMatrix, row, col, keyState ? 0 : 1);
    }
*/
    // Update the right keyboard matrix.
    KeyMatrix_Scan(rightMatrix, NULL);

    // Figure out which keymap is supposed to be the active one.
    uint8_t ActiveKeymap = KEYMAP_ID_NORMAL;
    uint8_t ColumnIndex = 0;
    for (uint8_t matrixId=0; matrixId<KEYMATRICES_NUM; matrixId++) {
        KeyMatrix_t *keyMatrix = keyMatrices + matrixId;
        for (uint8_t row=0; row<keyMatrix->RowNum; row++) {
            for (uint8_t col=0; col<keyMatrix->ColNum; col++) {
                if (GET_KEY_STATE_CURRENT(KeyMatrix_GetElement(keyMatrix, row, col))) {
                    uint8_t Action = pgm_read_byte(&KeyboardLayout[row][col+ColumnIndex][KEYMAP_ID_NORMAL][KEY_ACTION]);
                    if (Action == VIRTUAL_MODIFIER_KEY_MOUSE) {
                        ActiveKeymap = KEYMAP_ID_MOUSE;
                    } else if (Action == VIRTUAL_MODIFIER_KEY_FN && ActiveKeymap != KEYMAP_ID_MOUSE) {
                        ActiveKeymap = KEYMAP_ID_FN;
                    } else if (Action == VIRTUAL_MODIFIER_KEY_MOD && ActiveKeymap != KEYMAP_ID_MOUSE && ActiveKeymap != KEYMAP_ID_FN) {
                        ActiveKeymap = KEYMAP_ID_MOD;
                    }
                }
            }
        }
        ColumnIndex += keyMatrix->ColNum;
    }

    // Construct the keyboard report according to pressed keys.
    ColumnIndex = 0;
    for (uint8_t matrixId=0; matrixId<KEYMATRICES_NUM; matrixId++) {
        KeyMatrix_t *keyMatrix = keyMatrices + matrixId;
        for (uint8_t row=0; row<keyMatrix->RowNum; row++) {
            for (uint8_t col=0; col<keyMatrix->ColNum; col++) {
                if (GET_KEY_STATE_CURRENT(KeyMatrix_GetElement(keyMatrix, row, col)) && UsedKeyCodes<6) {
                    uint8_t *Key = KeyboardLayout[row][col+ColumnIndex][ActiveKeymap];
                    uint8_t Action = pgm_read_byte(&Key[KEY_ACTION]);
                    uint8_t Argument = pgm_read_byte(&Key[KEY_ARGUMENT]);
                    if (Action != VIRTUAL_MODIFIER_KEY_MOUSE &&
                        Action != VIRTUAL_MODIFIER_KEY_FN && Action != VIRTUAL_MODIFIER_KEY_MOD)
                    {
                        KeyboardReport->KeyCode[UsedKeyCodes++] = Action;
                        KeyboardReport->Modifier |= Argument;
                    }
                }
            }
        }
        ColumnIndex += keyMatrix->ColNum;
    }

    *ReportSize = sizeof(USB_KeyboardReport_Data_t);

    return false;
}

bool CreateMouseHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    USB_MouseReport_Data_t* MouseReport = (USB_MouseReport_Data_t*)ReportData;
    *ReportSize = sizeof(USB_MouseReport_Data_t);
    return true;

}

bool CreateGenericHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    uint8_t* Data = (uint8_t*)ReportData;
    *ReportSize = GENERIC_REPORT_SIZE;
    return false;
}

void ProcessGenericHIDReport(const void* ReportData, const uint16_t ReportSize)
{
    uint8_t* Data = (uint8_t*)ReportData;
    if (Data[0] == 66) {
        Jump_To_Bootloader();
    }
}
