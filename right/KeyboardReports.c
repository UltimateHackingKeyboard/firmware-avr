/** \file
 *
 *  HID report handlers for the keyboard.
 */

#include "KeyboardReports.h"

bool CreateKeyboardHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    // TODO: Implement proper debouncing algorithm that does not block.
    _delay_ms(10);  // Work around key bouncing.

    USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
    uint8_t UsedKeyCodes = 0;

/*
    // Update the left keyboard matrix.
    while (USART_HasByte()) {
        uint8_t Event = USART_ReceiveByte();
        uint8_t KeyState = GET_EVENT_STATE(Event);
        uint8_t KeyCode = GET_EVENT_PAYLOAD(Event);
        uint8_t Row = EXTRACT_KEYCODE_ROW(KeyCode, LEFT_COLS_NUM);
        uint8_t Col = EXTRACT_KEYCODE_COL(KeyCode, LEFT_COLS_NUM);
        KeyMatrix_SetElement(KEYMATRIX_LEFT, Row, Col, KeyState ? 0 : 1);
    }
*/

    // Update the right keyboard matrix.
    KeyMatrix_Scan(KEYMATRIX_RIGHT, NULL);


    // Figure out which keymap is active.
    uint8_t ActiveKeymap = KEYMAP_ID_NORMAL;
    uint8_t ColIndex = 0;

    for (uint8_t MatrixId=0; MatrixId<KEYMATRICES_NUM; MatrixId++) {
        KeyMatrix_t *KeyMatrix = KeyMatrices + MatrixId;
        uint8_t RowNum = KeyMatrix->Info->RowNum;
        uint8_t ColNum = KeyMatrix->Info->ColNum;
        for (uint8_t Row=0; Row<RowNum; Row++) {
            for (uint8_t Col=0; Col<ColNum; Col++) {
                if (GET_KEY_STATE_CURRENT(KeyMatrix_GetElement(KeyMatrix, Row, Col))) {
                    uint8_t Action = KeyboardLayout[Row][Col+ColIndex][KEYMAP_ID_NORMAL][KEY_ACTION];
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
        ColIndex += ColNum;
    }

    // Construct the keyboard report according to the pressed keys.
    ColIndex = 0;
    for (uint8_t MatrixId=0; MatrixId<KEYMATRICES_NUM; MatrixId++) {
        KeyMatrix_t *KeyMatrix = KeyMatrices + MatrixId;
        uint8_t RowNum = KeyMatrix->Info->RowNum;
        uint8_t ColNum = KeyMatrix->Info->ColNum;
        for (uint8_t Row=0; Row<RowNum; Row++) {
            for (uint8_t Col=0; Col<ColNum; Col++) {
                if (GET_KEY_STATE_CURRENT(KeyMatrix_GetElement(KeyMatrix, Row, Col)) && UsedKeyCodes<KEYBOARD_ROLLOVER) {

                    // TODO: Remove "const __flash" after putting the layout into the SRAM.
                    const __flash uint8_t *Key = KeyboardLayout[Row][Col+ColIndex][ActiveKeymap];
                    uint8_t Action = Key[KEY_ACTION];
                    uint8_t Argument = Key[KEY_ARGUMENT];
                    if (Action != VIRTUAL_MODIFIER_KEY_MOUSE &&
                        Action != VIRTUAL_MODIFIER_KEY_FN && Action != VIRTUAL_MODIFIER_KEY_MOD)
                    {
                        KeyboardReport->KeyCode[UsedKeyCodes++] = Action;
                        KeyboardReport->Modifier |= Argument;
                    }
                }
            }
        }
        ColIndex += ColNum;
    }

    *ReportSize = sizeof(USB_KeyboardReport_Data_t);

    return false;
}

bool CreateMouseHIDReport(void* ReportData, uint16_t* const ReportSize)
{
//    USB_MouseReport_Data_t* MouseReport = (USB_MouseReport_Data_t*)ReportData;
    *ReportSize = sizeof(USB_MouseReport_Data_t);
    return true;
}

bool CreateGenericHIDReport(void* ReportData, uint16_t* const ReportSize)
{
//    uint8_t* Data = (uint8_t*)ReportData;
    *ReportSize = GENERIC_REPORT_SIZE;
    return false;
}

void ProcessGenericHIDReport(const void* ReportData, const uint16_t ReportSize)
{
    uint8_t* Data = (uint8_t*)ReportData;
    uint8_t  Command = Data[0];

    if (Command == AGENT_COMMAND_REENUMERATE) {
        uint8_t ReenumerateAs = Data[1];
        Reenumerate(ReenumerateAs);
    }
}
