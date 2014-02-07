/** \file
 *
 *  HID report handlers for the keyboard.
 */

#include "KeyboardReports.h"

uint8_t PreviousLayer = LAYER_BASE;
uint8_t PreviousModifiers = NO_ARGUMENT;

static uint8_t MouseMovement;
static uint8_t MouseButtons;

static void ProcessMouseAction(uint8_t KeyAction);

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
    if (ActiveLayer != LAYER_BASE && ActiveLayer == PreviousLayer && UsedKeyCodes == 0) {
        KeyboardReport->Modifier |= PreviousModifiers;
    }

    *ReportSize = sizeof(USB_KeyboardReport_Data_t);

    PreviousLayer = ActiveLayer;
    PreviousModifiers = KeyboardReport->Modifier;

    return false;
}

uint8_t GetActiveLayer()
{
    uint8_t ActiveLayer = 0;
    uint8_t HighestLayerPriority = 0;
    uint8_t ColIndex = 0;

    for (uint8_t MatrixId=0; MatrixId<KEYMATRICES_NUM; MatrixId++) {
        KeyMatrix_t *KeyMatrix = KeyMatrices + MatrixId;
        uint8_t RowNum = KeyMatrix->Info->RowNum;
        uint8_t ColNum = KeyMatrix->Info->ColNum;
        for (uint8_t Row=0; Row<RowNum; Row++) {
            for (uint8_t Col=0; Col<ColNum; Col++) {
                if (KEY_STATE_IS_PRESSED(KeyMatrix_GetElement(KeyMatrix, Row, Col))) {
                    uint8_t KeyAction = KeyMap[Row][Col+ColIndex][LAYER_BASE][KEY_ACTION];
                    if (IS_KEY_ACTION_LAYER_SWITCHER(KeyAction)) {
                        uint8_t Layer = LAYER_SWITCHER_KEY_TO_LAYER(KeyAction);
                        uint8_t LayerPriority = LAYER_GET_PRIORITY(Layer);
                        if (LayerPriority > HighestLayerPriority) {
                            ActiveLayer = Layer;
                            HighestLayerPriority = LayerPriority;
                        }
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

    MouseMovement = 0;
    MouseButtons = 0;

    for (uint8_t MatrixId=0; MatrixId<KEYMATRICES_NUM; MatrixId++) {
        KeyMatrix_t *KeyMatrix = KeyMatrices + MatrixId;
        uint8_t RowNum = KeyMatrix->Info->RowNum;
        uint8_t ColNum = KeyMatrix->Info->ColNum;
        for (uint8_t Row=0; Row<RowNum; Row++) {
            for (uint8_t Col=0; Col<ColNum; Col++) {
                uint8_t KeyState = KeyMatrix_GetElement(KeyMatrix, Row, Col);
                if (KEY_STATE_IS_PRESSED(KeyState)) {
                    // TODO: Remove "const __flash" after putting the keymap into the SRAM.
                    const __flash uint8_t (*Key)[LAYERS_NUM][ITEM_NUM_PER_KEY] = &KeyMap[Row][Col+ColIndex];
                    const __flash uint8_t *ActiveKey = (*Key)[ActiveLayer];
                    const __flash uint8_t *BaseKey = (*Key)[LAYER_BASE];

                    uint8_t KeyAction = ActiveKey[KEY_ACTION];
                    uint8_t KeyArgument = ActiveKey[KEY_ARGUMENT];

                    if (IS_KEY_MODIFIER(BaseKey)) {
                        KeyboardReport->Modifier |= BaseKey[KEY_ARGUMENT];
                    } else {
                        // Suppress keys upon layer switcher key release.
                        if (PreviousLayer != LAYER_BASE && ActiveLayer == LAYER_BASE) {
                            KeyState |= KEY_STATE_MASK_SUPPRESSED;
                            KeyMatrix_SetElement(KeyMatrix, Row, Col, KeyState);
                        }

                        if (IS_KEY_ACTION_REGULAR(KeyAction)) {
                            // Add scancode to the array to be sent to the host.
                            if (!KEY_STATE_IS_SUPPRESSED(KeyState) && UsedKeyCodes < KEYBOARD_ROLLOVER) {
                                KeyboardReport->KeyCode[UsedKeyCodes++] = KeyAction;
                                KeyboardReport->Modifier |= KeyArgument;
                            }
                        } else if (IS_KEY_ACTION_MOUSE(KeyAction)) {
                            ProcessMouseAction(KeyAction);
                        }
                    }
                } else if (KEY_STATE_BECAME_RELEASED(KeyState) && KEY_STATE_IS_SUPPRESSED(KeyState)) {
                    // Unsuppress suppressed keys upon release.
                    KeyMatrix_SetElement(KeyMatrix, Row, Col, KeyState & ~KEY_STATE_MASK_SUPPRESSED);
                }
            }
        }
        ColIndex += ColNum;
    }

    return UsedKeyCodes;
}

static void ProcessMouseAction(uint8_t KeyAction)
{
    switch (KeyAction) {
        case MOUSE_MOVE_UP:
            MouseMovement |= MOUSE_STATE_UP;
            break;
        case MOUSE_MOVE_LEFT:
            MouseMovement |= MOUSE_STATE_LEFT;
            break;
        case MOUSE_MOVE_DOWN:
            MouseMovement |= MOUSE_STATE_DOWN;
            break;
        case MOUSE_MOVE_RIGHT:
            MouseMovement |= MOUSE_STATE_RIGHT;
            break;
        case MOUSE_CLICK_LEFT:
            MouseButtons |= MOUSE_STATE_LEFT_CLICK;
            break;
        case MOUSE_CLICK_MIDDLE:
            MouseButtons |= MOUSE_STATE_MIDDLE_CLICK;
            break;
        case MOUSE_CLICK_RIGHT:
            MouseButtons |= MOUSE_STATE_RIGHT_CLICK;
            break;
    }
}

bool CreateMouseHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    USB_MouseReport_Data_t* MouseReport = (USB_MouseReport_Data_t*)ReportData;

    if (MouseMovement & MOUSE_STATE_UP)
      MouseReport->Y = -1;
    else if (MouseMovement & MOUSE_STATE_DOWN)
      MouseReport->Y =  1;

    if (MouseMovement & MOUSE_STATE_LEFT)
      MouseReport->X = -1;
    else if (MouseMovement & MOUSE_STATE_RIGHT)
      MouseReport->X =  1;

    if (MouseButtons & MOUSE_STATE_LEFT_CLICK)
      MouseReport->Button |= (1 << 0);
    if (MouseButtons & MOUSE_STATE_MIDDLE_CLICK)
      MouseReport->Button |= (1 << 2);
    if (MouseButtons & MOUSE_STATE_RIGHT_CLICK)
      MouseReport->Button |= (1 << 1);

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
        uint8_t NewEnumerationMode = Data[1];
        Reenumerate(NewEnumerationMode);
    }
}
