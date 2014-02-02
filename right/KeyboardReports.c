/** \file
 *
 *  HID report handlers for the keyboard.
 */

#include "KeyboardReports.h"

uint8_t PreviousLayer = LAYER_ID_NORMAL;
uint8_t PreviousModifiers = NO_ARGUMENT;

bool CreateKeyboardHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    // TODO: Implement proper debouncing algorithm that does not block.
    _delay_ms(10);  // Work around key bouncing.

    USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;

    ProcessBufferedKeyStates();             // Update the left keyboard matrix.
    KeyMatrix_Scan(KEYMATRIX_RIGHT, NULL);  // Update the right keyboard matrix.

    uint8_t ActiveLayer = GetActiveLayer();
    uint8_t UsedKeyCodes = ConstructKeyboardReport(ActiveLayer, KeyboardReport, ReportSize);

    // When a layer switcher key gets pressed along with another key that produces some modifiers
    // and the accomanying key gets released then keep the related modifiers active a long as the
    // layer switcher key stays pressed.  Useful for alt-tab alternatives and the like.
    if (ActiveLayer != LAYER_ID_NORMAL && ActiveLayer == PreviousLayer && UsedKeyCodes == 0) {
        KeyboardReport->Modifier |= PreviousModifiers;
    }

    *ReportSize = sizeof(USB_KeyboardReport_Data_t);

    PreviousLayer = ActiveLayer;
    PreviousModifiers = KeyboardReport->Modifier;

    return false;
}

uint8_t GetActiveLayer()
{
    uint8_t ActiveLayer = LAYER_ID_NORMAL;
    uint8_t ColIndex = 0;

    for (uint8_t MatrixId=0; MatrixId<KEYMATRICES_NUM; MatrixId++) {
        KeyMatrix_t *KeyMatrix = KeyMatrices + MatrixId;
        uint8_t RowNum = KeyMatrix->Info->RowNum;
        uint8_t ColNum = KeyMatrix->Info->ColNum;
        for (uint8_t Row=0; Row<RowNum; Row++) {
            for (uint8_t Col=0; Col<ColNum; Col++) {
                if (GET_KEY_STATE_CURRENT(KeyMatrix_GetElement(KeyMatrix, Row, Col))) {
                    uint8_t Action = KeyMap[Row][Col+ColIndex][LAYER_ID_NORMAL][KEY_ACTION];
                    if (Action == LAYER_SWITCHER_KEY_MOUSE) {
                        ActiveLayer = LAYER_ID_MOUSE;
                    } else if (Action == LAYER_SWITCHER_KEY_FN && ActiveLayer != LAYER_ID_MOUSE) {
                        ActiveLayer = LAYER_ID_FN;
                    } else if (Action == LAYER_SWITCHER_KEY_MOD && ActiveLayer != LAYER_ID_MOUSE && ActiveLayer != LAYER_ID_FN) {
                        ActiveLayer = LAYER_ID_MOD;
                    }
                }
            }
        }
        ColIndex += ColNum;
    }

    return ActiveLayer;
}

uint8_t ConstructKeyboardReport(uint8_t ActiveLayer, USB_KeyboardReport_Data_t* KeyboardReport, uint16_t* const ReportSize)
{
    uint8_t UsedKeyCodes = 0;
    uint8_t ColIndex = 0;

    for (uint8_t MatrixId=0; MatrixId<KEYMATRICES_NUM; MatrixId++) {
        KeyMatrix_t *KeyMatrix = KeyMatrices + MatrixId;
        uint8_t RowNum = KeyMatrix->Info->RowNum;
        uint8_t ColNum = KeyMatrix->Info->ColNum;
        for (uint8_t Row=0; Row<RowNum; Row++) {
            for (uint8_t Col=0; Col<ColNum; Col++) {
                uint8_t KeyState = KeyMatrix_GetElement(KeyMatrix, Row, Col);
                if (GET_KEY_STATE_CURRENT(KeyState)) {
                    // TODO: Remove "const __flash" after putting the keymap into the SRAM.
                    const __flash uint8_t (*Key)[LAYERS_NUM][ITEM_NUM_PER_KEY] = &KeyMap[Row][Col+ColIndex];
                    const __flash uint8_t *ActiveKey = (*Key)[ActiveLayer];
                    const __flash uint8_t *NormalKey = (*Key)[LAYER_ID_NORMAL];

                    uint8_t KeyAction = ActiveKey[KEY_ACTION];
                    uint8_t KeyArgument = ActiveKey[KEY_ARGUMENT];

                    if (IS_KEY_MODIFIER(NormalKey)) {
                        KeyboardReport->Modifier |= NormalKey[KEY_ARGUMENT];
                    } else if (KeyAction != NO_ACTION && !IS_KEY_ACTION_LAYER_SWITCHER(KeyAction)) {
                        // Suppress keys upon layer switcher key release.
                        if (PreviousLayer != LAYER_ID_NORMAL && ActiveLayer == LAYER_ID_NORMAL) {
                            KeyState |= KEY_STATE_MASK_SUPPRESSED;
                            KeyMatrix_SetElement(KeyMatrix, Row, Col, KeyState);
                        }

                        // Add scancode to the array to be sent to the host.
                        if (!GET_KEY_STATE_SUPPRESSED(KeyState) && UsedKeyCodes < KEYBOARD_ROLLOVER) {
                            KeyboardReport->KeyCode[UsedKeyCodes++] = KeyAction;
                            KeyboardReport->Modifier |= KeyArgument;
                        }
                    }
                } else if  // Unsuppress suppressed keys upon release.
                    (GET_KEY_STATE_PREV(KeyState) &&
                    !GET_KEY_STATE_CURRENT(KeyState) &&
                     GET_KEY_STATE_SUPPRESSED(KeyState))
                {
                    KeyMatrix_SetElement(KeyMatrix, Row, Col, KeyState & ~KEY_STATE_MASK_SUPPRESSED);
                }
            }
        }
        ColIndex += ColNum;
    }

    return UsedKeyCodes;
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
