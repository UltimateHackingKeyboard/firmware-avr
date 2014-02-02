/** \file
 *
 *  HID report handlers for the keyboard.
 */

#include "KeyboardReports.h"

uint8_t PreviousKeymap = KEYMAP_ID_NORMAL;
uint8_t PreviousModifiers = NO_ARGUMENT;

bool CreateKeyboardHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    // TODO: Implement proper debouncing algorithm that does not block.
    _delay_ms(10);  // Work around key bouncing.

    USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
    uint8_t UsedKeyCodes = 0;

    ProcessBufferedKeyStates();

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
                uint8_t KeyState = KeyMatrix_GetElement(KeyMatrix, Row, Col);
                if (GET_KEY_STATE_CURRENT(KeyState)) {
                    // TODO: Remove "const __flash" after putting the layout into the SRAM.
                    const __flash uint8_t (*Key)[KEYMAPS_NUM][ITEM_NUM_PER_KEY] = &KeyboardLayout[Row][Col+ColIndex];
                    const __flash uint8_t *ActiveKey = (*Key)[ActiveKeymap];
                    const __flash uint8_t *NormalKey = (*Key)[KEYMAP_ID_NORMAL];

                    uint8_t Action = ActiveKey[KEY_ACTION];
                    uint8_t Argument = ActiveKey[KEY_ARGUMENT];

                    if  // Check modifier keys on the normal keymap.
                       (NormalKey[KEY_ACTION] == NO_ACTION &&
                        NormalKey[KEY_ARGUMENT] != NO_ARGUMENT)
                    {
                        KeyboardReport->Modifier |= NormalKey[KEY_ARGUMENT];
                    } else if  // Check the keys of the actual keymap.
                       (Action != NO_ACTION &&
                        Action != VIRTUAL_MODIFIER_KEY_MOUSE &&
                        Action != VIRTUAL_MODIFIER_KEY_FN &&
                        Action != VIRTUAL_MODIFIER_KEY_MOD)
                    {
                        // Suppress keys upon virtual modifier release.
                        if (PreviousKeymap != KEYMAP_ID_NORMAL && ActiveKeymap == KEYMAP_ID_NORMAL) {
                            KeyState |= KEY_STATE_MASK_SUPPRESSED;
                            KeyMatrix_SetElement(KeyMatrix, Row, Col, KeyState);
                        }

                        // Add scancode to the array to be sent to the host.
                        if (!GET_KEY_STATE_SUPPRESSED(KeyState) && UsedKeyCodes < KEYBOARD_ROLLOVER) {
                            KeyboardReport->KeyCode[UsedKeyCodes++] = Action;
                            KeyboardReport->Modifier |= Argument;
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

    // When a virtual modifier gets pressed along with another key that produces some actual
    // modifiers and the accomanying key gets released then keep the related actual modifiers active
    // as long as the virtual modifier gets released.  Useful for alt-tab alternatives and the like.
    if (ActiveKeymap != KEYMAP_ID_NORMAL && ActiveKeymap == PreviousKeymap && UsedKeyCodes == 0) {
        KeyboardReport->Modifier |= PreviousModifiers;
    }

    *ReportSize = sizeof(USB_KeyboardReport_Data_t);

    PreviousKeymap = ActiveKeymap;
    PreviousModifiers = KeyboardReport->Modifier;

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
