/** \file
 *
 *  HID report handlers for the keyboard.
 */

#include "KeyboardReports.h"

uint8_t PreviousLayer = LAYER_BASE;
uint8_t PreviousModifiers = NO_ARGUMENT;

static uint8_t MouseMovement;
static uint8_t MouseButtons;
static uint8_t MouseMomentum = 0;
static int8_t VerticalWheelMovement = 0;
static int8_t HorizontalWheelMovement = 0;

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
    VerticalWheelMovement = 0;
    HorizontalWheelMovement = 0;

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

uint8_t IsPreviousActionMouseWheelAction = false;
uint8_t MouseWheelDivisorCounter = 0;

static void ProcessMouseAction(uint8_t KeyAction)
{
    uint8_t IsMouseWheelAction = false;

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
        case MOUSE_WHEEL_UP:
            IsMouseWheelAction = true;
            VerticalWheelMovement = MOUSE_WHEEL_SPEED;
            break;
        case MOUSE_WHEEL_DOWN:
            IsMouseWheelAction = true;
            VerticalWheelMovement = -MOUSE_WHEEL_SPEED;
            break;
        case MOUSE_WHEEL_LEFT:
            IsMouseWheelAction = true;
            HorizontalWheelMovement = -MOUSE_WHEEL_SPEED;
            break;
        case MOUSE_WHEEL_RIGHT:
            IsMouseWheelAction = true;
            HorizontalWheelMovement = MOUSE_WHEEL_SPEED;
            break;
    }

    if (IsMouseWheelAction == IsPreviousActionMouseWheelAction) {
        MouseWheelDivisorCounter++;
        if (MouseWheelDivisorCounter == MOUSE_WHEEL_DIVISOR) {
            MouseWheelDivisorCounter = 0;
        }
    } else {
        MouseWheelDivisorCounter = 0;
    }

    if (!(IsMouseWheelAction && MouseWheelDivisorCounter == 0)) {
        HorizontalWheelMovement = 0;
        VerticalWheelMovement = 0;
    }

    IsPreviousActionMouseWheelAction = IsMouseWheelAction;
}

bool CreateMouseHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    USB_WheeledMouseReport_Data_t* MouseReport = (USB_WheeledMouseReport_Data_t*)ReportData;

    if (MouseMovement == 0) {
        MouseMomentum = 0;
    } else {
        if (MouseMomentum <= UINT8_MAX - MOUSE_ACCELERATION) {
            MouseMomentum += MOUSE_ACCELERATION;
        }

        uint8_t MouseSpeed = MouseMomentum/MOUSE_SPEED_DIVISOR + 1;

        if (MouseSpeed > MOUSE_MAX_SPEED) {
            MouseSpeed = MOUSE_MAX_SPEED;
        }

        if (MouseMovement & MOUSE_STATE_UP) {
            MouseReport->Y = -MouseSpeed;
        } else if (MouseMovement & MOUSE_STATE_DOWN) {
            MouseReport->Y = MouseSpeed;
        }

        if (MouseMovement & MOUSE_STATE_LEFT) {
            MouseReport->X = -MouseSpeed;
        } else if (MouseMovement & MOUSE_STATE_RIGHT) {
            MouseReport->X = MouseSpeed + 1;
        }
    }

    MouseReport->Button = MouseButtons;
    MouseReport->VerticalWheelMovement = VerticalWheelMovement;
    MouseReport->HorizontalWheelMovement = HorizontalWheelMovement;
    *ReportSize = sizeof(USB_WheeledMouseReport_Data_t);
    return true;
}

uint8_t RamContent[RAM_CONTENT_SIZE];

bool CreateGenericHIDReport(void* ReportData, uint16_t* const ReportSize)
{
    uint8_t* Data = (uint8_t*)ReportData;
    *ReportSize = GENERIC_REPORT_SIZE;
    EEPROM_Read(Data, RAM_CONTENT_SIZE);
    return true;
}

void ProcessGenericHIDReport(const void* ReportData, const uint16_t ReportSize)
{
    uint8_t* Data = (uint8_t*)ReportData;
    uint8_t  Command = Data[0];
    uint8_t NewEnumerationMode = Data[1];

    switch (Command) {
        case AGENT_COMMAND_REENUMERATE:
            Wormhole->EnumerationMode = NewEnumerationMode;
            ShouldReenumerate = true;
            break;
        case AGENT_COMMAND_WRITE_TO_RAM:
            EEPROM_Write(Data+1, RAM_CONTENT_SIZE);
            break;
        case AGENT_COMMAND_READ_FROM_RAM:
            break;
    }
}
