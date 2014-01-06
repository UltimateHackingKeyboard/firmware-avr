/** \file
 *
 *  Main file for the keyboard function of the right keyboard half.
 */

#include "Keyboard.h"
#include "../KeyMatrix.c"

#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ROW_NUM 5
#define COL_NUM 7

#define KEYMATRICES_NUM 2

KeyMatrix_t *leftMatrix;
KeyMatrix_t *rightMatrix;
KeyMatrix_t keyMatrices[KEYMATRICES_NUM];

uint8_t normalKeyMap[5][13] = {
    {  // Row 1
        HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE,
        HID_KEYBOARD_SC_1_AND_EXCLAMATION,
        HID_KEYBOARD_SC_2_AND_AT,
        HID_KEYBOARD_SC_3_AND_HASHMARK,
        HID_KEYBOARD_SC_4_AND_DOLLAR,
        HID_KEYBOARD_SC_6_AND_CARET,
        HID_KEYBOARD_SC_7_AND_AND_AMPERSAND,
        HID_KEYBOARD_SC_8_AND_ASTERISK,
        HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS,
        HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS,
        HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE,
        HID_KEYBOARD_SC_EQUAL_AND_PLUS,
        HID_KEYBOARD_SC_BACKSPACE,
    }, {  // Row 2
        HID_KEYBOARD_SC_TAB,
        HID_KEYBOARD_SC_Q,
        HID_KEYBOARD_SC_W,
        HID_KEYBOARD_SC_E,
        HID_KEYBOARD_SC_R,
        HID_KEYBOARD_SC_5_AND_PERCENTAGE,
        HID_KEYBOARD_SC_U,
        HID_KEYBOARD_SC_I,
        HID_KEYBOARD_SC_O,
        HID_KEYBOARD_SC_P,
        HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE,
        HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE,
        HID_KEYBOARD_SC_BACKSLASH_AND_PIPE,
    }, {  // Row 3
        HID_KEYBOARD_SC_CAPS_LOCK,
        HID_KEYBOARD_SC_A,
        HID_KEYBOARD_SC_S,
        HID_KEYBOARD_SC_D,
        HID_KEYBOARD_SC_F,
        HID_KEYBOARD_SC_T,
        HID_KEYBOARD_SC_Y,
        HID_KEYBOARD_SC_J,
        HID_KEYBOARD_SC_K,
        HID_KEYBOARD_SC_L,
        HID_KEYBOARD_SC_SEMICOLON_AND_COLON,
        HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE,
        HID_KEYBOARD_SC_ENTER,
    }, {  // Row 4
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_SHIFT,
        HID_KEYBOARD_SC_Z,
        HID_KEYBOARD_SC_X,
        HID_KEYBOARD_SC_C,
        HID_KEYBOARD_SC_V,
        HID_KEYBOARD_SC_G,
        HID_KEYBOARD_SC_H,
        HID_KEYBOARD_SC_N,
        HID_KEYBOARD_SC_M,
        HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,
        HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,
        HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_SHIFT,
    }, {  // Row 5
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_GUI,  // Left Super
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_CONTROL,
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_ALT,
        NO_KEY,
        NO_KEY,  // Hyper
        HID_KEYBOARD_SC_B,
        NO_KEY,
        NO_KEY,
        HID_KEYBOARD_SC_SPACE,
        NO_KEY,
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_ALT,
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_CONTROL,
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_GUI,  // Right Super
    }
};

uint8_t hyperKeyMap[5][13] = {
    {  // Row 1
        HID_KEYBOARD_SC_ESCAPE,  // HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE
        HID_KEYBOARD_SC_F1,  // HID_KEYBOARD_SC_1_AND_EXCLAMATION
        HID_KEYBOARD_SC_F2,  // HID_KEYBOARD_SC_2_AND_AT
        HID_KEYBOARD_SC_F3,  // HID_KEYBOARD_SC_3_AND_HASHMARK
        HID_KEYBOARD_SC_F4,  // HID_KEYBOARD_SC_4_AND_DOLLAR
        HID_KEYBOARD_SC_F6,  // HID_KEYBOARD_SC_6_AND_CARET
        HID_KEYBOARD_SC_F7,  // HID_KEYBOARD_SC_7_AND_AND_AMPERSAND
        HID_KEYBOARD_SC_F8,  // HID_KEYBOARD_SC_8_AND_ASTERISK
        HID_KEYBOARD_SC_F9,  // HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS
        HID_KEYBOARD_SC_F10,  // HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS
        HID_KEYBOARD_SC_F11,  // HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE
        HID_KEYBOARD_SC_F12,  // HID_KEYBOARD_SC_EQUAL_AND_PLUS
        NO_KEY,  // HID_KEYBOARD_SC_BACKSPACE
    }, {  // Row 2
        NO_KEY,  // HID_KEYBOARD_SC_TAB
        NO_KEY,  // HID_KEYBOARD_SC_Q
        NO_KEY,  // HID_KEYBOARD_SC_W
        HID_KEYBOARD_SC_TAB,  // HID_KEYBOARD_SC_E
        HID_KEYBOARD_SC_TAB,  // HID_KEYBOARD_SC_R
        HID_KEYBOARD_SC_F5,  // HID_KEYBOARD_SC_5_AND_PERCENTAGE
        HID_KEYBOARD_SC_HOME,  // HID_KEYBOARD_SC_U
        HID_KEYBOARD_SC_UP_ARROW,  // HID_KEYBOARD_SC_I
        HID_KEYBOARD_SC_END,  // HID_KEYBOARD_SC_O
        HID_KEYBOARD_SC_DELETE,  // HID_KEYBOARD_SC_P
        NO_KEY,  // HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE
        NO_KEY,  // HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE
        NO_KEY,  // HID_KEYBOARD_SC_BACKSLASH_AND_PIPE
    }, {  // Row 3
        NO_KEY,  // HID_KEYBOARD_SC_CAPS_LOCK
        HID_KEYBOARD_SC_TAB,  // HID_KEYBOARD_SC_A
        HID_KEYBOARD_SC_A,  // HID_KEYBOARD_SC_S
        NO_KEY,  // HID_KEYBOARD_SC_D
        HID_KEYBOARD_SC_S,  // HID_KEYBOARD_SC_F
        NO_KEY,  // HID_KEYBOARD_SC_T
        HID_KEYBOARD_SC_PAGE_UP,  // HID_KEYBOARD_SC_Y
        HID_KEYBOARD_SC_LEFT_ARROW,  // HID_KEYBOARD_SC_J
        HID_KEYBOARD_SC_DOWN_ARROW,  // HID_KEYBOARD_SC_K
        HID_KEYBOARD_SC_RIGHT_ARROW,  // HID_KEYBOARD_SC_L
        HID_KEYBOARD_SC_ENTER,  // HID_KEYBOARD_SC_SEMICOLON_AND_COLON
        NO_KEY,  // HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE
        NO_KEY,  // HID_KEYBOARD_SC_ENTER
    }, {  // Row 4
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_SHIFT
        NO_KEY,  // HID_KEYBOARD_SC_Z
        NO_KEY,  // HID_KEYBOARD_SC_X
        NO_KEY,  // HID_KEYBOARD_SC_C
        NO_KEY,  // HID_KEYBOARD_SC_V
        NO_KEY,  // HID_KEYBOARD_SC_G
        HID_KEYBOARD_SC_PAGE_DOWN,  // HID_KEYBOARD_SC_H
        HID_KEYBOARD_SC_ESCAPE,  // HID_KEYBOARD_SC_N
        NO_KEY,  // HID_KEYBOARD_SC_M
        NO_KEY,  // HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN
        NO_KEY,  // HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN
        NO_KEY,  // HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_SHIFT
    }, {  // Row 5
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_GUI
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_CONTROL
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_ALT
        NO_KEY,  // NO_KEY
        NO_KEY,  // NO_KEY  // Hyper
        NO_KEY,  // HID_KEYBOARD_SC_B,
        NO_KEY,  // NO_KEY
        NO_KEY,  // NO_KEY
        NO_KEY,  // HID_KEYBOARD_SC_SPACE
        NO_KEY,  // NO_KEY
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_ALT
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_CONTROL
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_GUI
    }
};

uint8_t hyperModifierMap[5][13] = {
    {  // Row 1
        NO_KEY,  // HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE
        NO_KEY,  // HID_KEYBOARD_SC_1_AND_EXCLAMATION
        NO_KEY,  // HID_KEYBOARD_SC_2_AND_AT
        NO_KEY,  // HID_KEYBOARD_SC_3_AND_HASHMARK
        NO_KEY,  // HID_KEYBOARD_SC_4_AND_DOLLAR
        NO_KEY,  // HID_KEYBOARD_SC_6_AND_CARET
        NO_KEY,  // HID_KEYBOARD_SC_7_AND_AND_AMPERSAND
        NO_KEY,  // HID_KEYBOARD_SC_8_AND_ASTERISK
        NO_KEY,  // HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS
        NO_KEY,  // HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS
        NO_KEY,  // HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE
        NO_KEY,  // HID_KEYBOARD_SC_EQUAL_AND_PLUS
        NO_KEY,  // HID_KEYBOARD_SC_BACKSPACE
    }, {  // Row 2
        NO_KEY,  // HID_KEYBOARD_SC_TAB
        NO_KEY,  // HID_KEYBOARD_SC_Q
        HID_KEYBOARD_MODIFIER_LEFTCTRL,  // HID_KEYBOARD_SC_W
        HID_KEYBOARD_MODIFIER_LEFTCTRL | HID_KEYBOARD_MODIFIER_LEFTSHIFT,  // HID_KEYBOARD_SC_E
        HID_KEYBOARD_MODIFIER_LEFTCTRL,  // HID_KEYBOARD_SC_R
        NO_KEY,  // HID_KEYBOARD_SC_5_AND_PERCENTAGE
        NO_KEY,  // HID_KEYBOARD_SC_U
        NO_KEY,  // HID_KEYBOARD_SC_I
        NO_KEY,  // HID_KEYBOARD_SC_O
        NO_KEY,  // HID_KEYBOARD_SC_P
        NO_KEY,  // HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE
        NO_KEY,  // HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE
        NO_KEY,  // HID_KEYBOARD_SC_BACKSLASH_AND_PIPE
    }, {  // Row 3
        NO_KEY,  // HID_KEYBOARD_SC_CAPS_LOCK
        HID_KEYBOARD_MODIFIER_LEFTALT,  // HID_KEYBOARD_SC_A
        HID_KEYBOARD_MODIFIER_LEFTCTRL | HID_KEYBOARD_MODIFIER_LEFTALT,  // HID_KEYBOARD_SC_S
        NO_KEY,  // HID_KEYBOARD_SC_D
        HID_KEYBOARD_MODIFIER_LEFTCTRL | HID_KEYBOARD_MODIFIER_LEFTALT,  // HID_KEYBOARD_SC_F
        NO_KEY,  // HID_KEYBOARD_SC_T
        NO_KEY,  // HID_KEYBOARD_SC_Y
        NO_KEY,  // HID_KEYBOARD_SC_J
        NO_KEY,  // HID_KEYBOARD_SC_K
        NO_KEY,  // HID_KEYBOARD_SC_L
        NO_KEY,  // HID_KEYBOARD_SC_SEMICOLON_AND_COLON
        NO_KEY,  // HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE
        NO_KEY,  // HID_KEYBOARD_SC_ENTER
    }, {  // Row 4
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_SHIFT
        NO_KEY,  // HID_KEYBOARD_SC_Z
        NO_KEY,  // HID_KEYBOARD_SC_X
        NO_KEY,  // HID_KEYBOARD_SC_C
        NO_KEY,  // HID_KEYBOARD_SC_V
        NO_KEY,  // HID_KEYBOARD_SC_G
        NO_KEY,  // HID_KEYBOARD_SC_H
        NO_KEY,  // HID_KEYBOARD_SC_N
        NO_KEY,  // HID_KEYBOARD_SC_M
        NO_KEY,  // HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN
        NO_KEY,  // HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN
        NO_KEY,  // HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_SHIFT
    }, {  // Row 5
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_GUI
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_CONTROL
        NO_KEY,  // HID_KEYBOARD_SC_LEFT_ALT
        NO_KEY,  // NO_KEY
        NO_KEY,  // NO_KEY  // Hyper
        NO_KEY,  // HID_KEYBOARD_SC_B,
        NO_KEY,  // NO_KEY
        NO_KEY,  // NO_KEY
        NO_KEY,  // HID_KEYBOARD_SC_SPACE
        NO_KEY,  // NO_KEY
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_ALT
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_CONTROL
        NO_KEY,  // HID_KEYBOARD_SC_RIGHT_GUI
    }
};

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** Buffer to hold the previously generated Mouse HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevMouseHIDReportBuffer[sizeof(USB_MouseReport_Data_t)];

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
    {
        .Config =
            {
                .InterfaceNumber              = INTERFACE_ID_Keyboard,
                .ReportINEndpoint             =
                    {
                        .Address              = KEYBOARD_IN_EPADDR,
                        .Size                 = HID_EPSIZE,
                        .Banks                = 1,
                    },
                .PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
                .PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
            },
    };

USB_ClassInfo_HID_Device_t Mouse_HID_Interface =
    {
        .Config =
            {
                .InterfaceNumber              = INTERFACE_ID_Mouse,
                .ReportINEndpoint             =
                    {
                        .Address              = MOUSE_IN_EPADDR,
                        .Size                 = HID_EPSIZE,
                        .Banks                = 1,
                    },
                .PrevReportINBuffer           = PrevMouseHIDReportBuffer,
                .PrevReportINBufferSize       = sizeof(PrevMouseHIDReportBuffer),
            },
    };

USB_ClassInfo_HID_Device_t Generic_HID_Interface =
    {
        .Config =
            {
                .InterfaceNumber              = INTERFACE_ID_GenericHID,
                .ReportINEndpoint             =
                    {
                        .Address              = GENERIC_IN_EPADDR,
                        .Size                 = GENERIC_EPSIZE,
                        .Banks                = 1,
                    },
                .PrevReportINBuffer           = PrevHIDReportBuffer,
                .PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
            },
    };

int KeyboardMainLoop(void)
{
    // Initialize USART
    UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
    UBRR1H = BAUD_PRESCALE >> 8;
    UBRR1L = BAUD_PRESCALE;

    leftMatrix = keyMatrices + 0;
    rightMatrix = keyMatrices + 1;

    Pin_t row_pins[ROW_NUM] = {
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB7 },
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB6 },
        { .Direction=&DDRB, .Name=&PINB, .Number=PINB5 },
        { .Direction=&DDRD, .Name=&PIND, .Number=PIND5 },
        { .Direction=&DDRD, .Name=&PIND, .Number=PIND6 },
    };

    Port_t column_ports[COL_NUM] = {
        { .Direction=&DDRC, .Name=&PORTC, .Number=PORTC6 },
        { .Direction=&DDRC, .Name=&PORTC, .Number=PORTC7 },
        { .Direction=&DDRB, .Name=&PORTB, .Number=PORTB4 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD4 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD1 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD0 },
        { .Direction=&DDRC, .Name=&PORTC, .Number=PORTC2 },
    };


    KeyMatrix_Init(rightMatrix, ROW_NUM, COL_NUM);
    KeyMatrix_SetColPortsAndRowPins(rightMatrix, column_ports, row_pins);

    KeyMatrix_Init(leftMatrix, 5, 6);

    for (;;)
    {
        HID_Device_USBTask(&Keyboard_HID_Interface);
        HID_Device_USBTask(&Mouse_HID_Interface);
        HID_Device_USBTask(&Generic_HID_Interface);
        USB_USBTask();
    }
}

bool EVENT_USB_Keyboard_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;
    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);
    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Mouse_HID_Interface);
    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);
    USB_Device_EnableSOFEvents();
    return ConfigSuccess;
}

void EVENT_USB_Keyboard_Device_ControlRequest(void)
{
    HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
    HID_Device_ProcessControlRequest(&Mouse_HID_Interface);
    HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

void EVENT_USB_Device_StartOfFrame(void)
{
    HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
    HID_Device_MillisecondElapsed(&Mouse_HID_Interface);
    HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}


uint8_t GetGlobalMatrixColByPartMatrixCol(KeyMatrix_t *keyMatrix, uint8_t col)
{
    if (keyMatrix == rightMatrix) {
        col = rightMatrix->ColNum - col + leftMatrix->ColNum-1;
    }
    return col;
}

uint8_t IsKeyPressed(KeyMatrix_t *keyMatrices, uint8_t row, uint8_t col)
{
    return GET_KEY_STATE_CURRENT(KeyMatrix_GetElement(keyMatrices+(col < 6 ? 0 : 1),
                                                      row,
                                                      (col < 6 ? col : 6-(col-6))) );
}

uint8_t KeyCodeToScanCode(KeyMatrix_t *keyMatrix, uint8_t row, uint8_t col)
{
    if (keyMatrix == rightMatrix) {
        col = rightMatrix->ColNum - col + leftMatrix->ColNum-1;
    }
    return IS_KEY_PRESSED_HYPER(keyMatrices) ? hyperKeyMap[row][col] : normalKeyMap[row][col];
}

uint8_t KeyModifierByScanCode(KeyMatrix_t *keyMatrix, uint8_t row, uint8_t col)
{
    if (keyMatrix == rightMatrix) {
        col = rightMatrix->ColNum - col + leftMatrix->ColNum-1;
    }
    return IS_KEY_PRESSED_HYPER(keyMatrices) ? hyperModifierMap[row][col] : 0;
}

uint8_t GetScanCodeByMatrixIdRowCol(KeyMatrix_t *keyMatrices, uint8_t matrixId, uint8_t row, uint8_t col)
{
    return KeyCodeToScanCode(keyMatrices+matrixId, row, col);
}

uint8_t HasEvent()
{
    return UCSR1A & (1 << RXC1);
}

uint8_t ReadEvent()
{
    return UDR1;
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent
 *
 *  \return Boolean true to force the sending of the report, false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, uint8_t* const ReportID,
                                         const uint8_t ReportType, void* ReportData, uint16_t* const ReportSize)
{
    /* Determine which interface must have its report generated */
    if (HIDInterfaceInfo == &Keyboard_HID_Interface) {
        _delay_ms(10);  // Work around key bouncing.

        USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
        uint8_t UsedKeyCodes = 0;

        if (HasEvent()) {
            uint8_t event = ReadEvent();
            uint8_t isKeyPressed = GET_EVENT_STATE(event);
            event = GET_EVENT_PAYLOAD(event);
            uint8_t row = event / leftMatrix->ColNum;
            uint8_t col = event % leftMatrix->ColNum;
            KeyMatrix_SetElement(leftMatrix, row, col, isKeyPressed ? 0 : 1);
        }

        KeyMatrix_Scan(rightMatrix, NULL);

        for (uint8_t matrixId=0; matrixId<KEYMATRICES_NUM; matrixId++) {
            KeyMatrix_t *keyMatrix = keyMatrices + matrixId;
            for (uint8_t row=0; row<keyMatrix->RowNum; row++) {
                for (uint8_t col=0; col<keyMatrix->ColNum; col++) {
                    if (GET_KEY_STATE_CURRENT(KeyMatrix_GetElement(keyMatrix, row, col))) {
                        uint8_t scanCode = KeyCodeToScanCode(keyMatrix, row, col);
                        if (scanCode != NO_KEY) {
                            KeyboardReport->KeyCode[UsedKeyCodes++] = scanCode;
                            KeyboardReport->Modifier |= KeyModifierByScanCode(keyMatrix, row, col);
                        }
                    }
                }
            }
        }


        if (IS_KEY_PRESSED_LEFT_SHIFT(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
        }
        if (IS_KEY_PRESSED_RIGHT_SHIFT(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTSHIFT;
        }
        if (IS_KEY_PRESSED_LEFT_SUPER(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTCTRL;
        }
        if (IS_KEY_PRESSED_LEFT_CONTROL(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTCTRL;
        }
        if (IS_KEY_PRESSED_LEFT_ALT(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTALT;
        }
        if (IS_KEY_PRESSED_RIGHT_ALT(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTALT;
        }
        if (IS_KEY_PRESSED_RIGHT_CONTROL(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTCTRL;
        }
        if (IS_KEY_PRESSED_RIGHT_SUPER(keyMatrices)) {
            KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTGUI;
        }

        *ReportSize = sizeof(USB_KeyboardReport_Data_t);

        return false;
    } else if (HIDInterfaceInfo == &Mouse_HID_Interface) {
 //       USB_MouseReport_Data_t* MouseReport = (USB_MouseReport_Data_t*)ReportData;
 //       *ReportSize = sizeof(USB_MouseReport_Data_t);
        return true;
    } else if (HIDInterfaceInfo == &Generic_HID_Interface) {
//        uint8_t* Data = (uint8_t*)ReportData;
//        *ReportSize = GENERIC_REPORT_SIZE;
        return false;
    }
}

uint32_t Boot_Key ATTR_NO_INIT;
#define MAGIC_BOOT_KEY            0xDC42ACCA
#define FLASH_SIZE_BYTES          32768
#define BOOTLOADER_SEC_SIZE_BYTES 4096
#define BOOTLOADER_START_ADDRESS  (FLASH_SIZE_BYTES - BOOTLOADER_SEC_SIZE_BYTES)

void Bootloader_Jump_Check(void) ATTR_INIT_SECTION(3);
void Bootloader_Jump_Check(void)
{
    // If the reset source was the bootloader and the key is correct, clear it and jump to the bootloader
    if ((MCUSR & (1 << WDRF)) && (Boot_Key == MAGIC_BOOT_KEY))
    {
        Boot_Key = 0;
        ((void (*)(void))BOOTLOADER_START_ADDRESS)();
    }
}

void Jump_To_Bootloader(void)
{
    // If USB is used, detach from the bus and reset it
    USB_Disable();

    // Disable all interrupts
    cli();

    // Wait two seconds for the USB detachment to register on the host
    Delay_MS(2000);

    // Set the bootloader key to the magic value and force a reset
    Boot_Key = MAGIC_BOOT_KEY;
    wdt_enable(WDTO_250MS);
    for (;;);
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
    uint8_t* Data = (uint8_t*)ReportData;
    if (HIDInterfaceInfo == &Generic_HID_Interface) {
        if (Data[0] == 66) {
            Jump_To_Bootloader();
        }
    }
}
