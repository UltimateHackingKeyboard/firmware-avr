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

uint8_t VirtualModifierKeyCache[VIRUTAL_MODIFIER_TYPES_NUM][MAX_KEYS_PER_VIRTUAL_MODIFIER_TYPE];

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

    KeyMatrix_Init(leftMatrix, 5, 7);

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

void InitVirtualModifierKeyCache()
{
    uint8_t ModArrayPosition = 0;
    uint8_t FnArrayPosition = 0;
    uint8_t MouseArrayPosition = 0;

    for (uint8_t KeyNumber=0; KeyNumber<KEYS_NUM; KeyNumber++) {
        uint8_t Action = KeyboardLayout[KeyNumber][KEYMAP_ID_NORMAL][KEY_ACTION];
        if (Action == VIRTUAL_MODIFIER_KEY_MOD && ModArrayPosition < MAX_KEYS_PER_VIRTUAL_MODIFIER_TYPE) {
            VirtualModifierKeyCache[CACHED_VIRTUAL_MODIFIER_KEY_MOD][ModArrayPosition++] = KeyNumber;
        } else if (Action == VIRTUAL_MODIFIER_KEY_FN && FnArrayPosition < MAX_KEYS_PER_VIRTUAL_MODIFIER_TYPE) {
            VirtualModifierKeyCache[CACHED_VIRTUAL_MODIFIER_KEY_FN][FnArrayPosition++] = KeyNumber;
        } else if (Action == VIRTUAL_MODIFIER_KEY_MOUSE && MouseArrayPosition < MAX_KEYS_PER_VIRTUAL_MODIFIER_TYPE) {
            VirtualModifierKeyCache[CACHED_VIRTUAL_MODIFIER_KEY_MOUSE][MouseArrayPosition++] = KeyNumber;
        }
    }

    while (ModArrayPosition<MAX_KEYS_PER_VIRTUAL_MODIFIER_TYPE) {
        VirtualModifierKeyCache[CACHED_VIRTUAL_MODIFIER_KEY_MOD][ModArrayPosition++] = VIRTUAL_MODIFIER_KEY_NONE;
    }
    while (FnArrayPosition<MAX_KEYS_PER_VIRTUAL_MODIFIER_TYPE) {
        VirtualModifierKeyCache[CACHED_VIRTUAL_MODIFIER_KEY_FN][FnArrayPosition++] = VIRTUAL_MODIFIER_KEY_NONE;
    }
    while (MouseArrayPosition<MAX_KEYS_PER_VIRTUAL_MODIFIER_TYPE) {
        VirtualModifierKeyCache[CACHED_VIRTUAL_MODIFIER_KEY_MOUSE][MouseArrayPosition++] = VIRTUAL_MODIFIER_KEY_NONE;
    }
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, uint8_t* const ReportID,
                                         const uint8_t ReportType, void* ReportData, uint16_t* const ReportSize)
{
    /* Determine which interface must have its report generated */
    if (HIDInterfaceInfo == &Keyboard_HID_Interface) {
        _delay_ms(10);  // Work around key bouncing.

        USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
        uint8_t UsedKeyCodes = 0;

        // Update the left keyboard matrix.
        while (HasEvent()) {
            uint8_t event = ReadEvent();
            uint8_t keyState = GET_EVENT_STATE(event);
            uint8_t keyId = GET_EVENT_PAYLOAD(event);
            uint8_t row = keyId / leftMatrix->ColNum;
            uint8_t col = keyId % leftMatrix->ColNum;
            KeyMatrix_SetElement(leftMatrix, row, col, keyState ? 0 : 1);
        }

        // Update the right keyboard matrix.
        KeyMatrix_Scan(rightMatrix, NULL);

        // Figure out which keymap is supposed to be the active one.
        // check mouse keymap, fn keymap, mod keymap


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
