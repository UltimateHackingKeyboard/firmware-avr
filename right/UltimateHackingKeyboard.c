/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the Keyboard demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include <avr/io.h>
#include "../KeyMatrix.c"
#include "UltimateHackingKeyboard.h"
#include "KeyPressMacros.h"
#include "keymaps/normalKeyMap.h"
#include "keymaps/hyperKeyMap.h"
#include "keymaps/hyperModifierMap.h"

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** Buffer to hold the previously generated Mouse HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevMouseHIDReportBuffer[sizeof(USB_MouseReport_Data_t)];

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the keyboard HID
 *  interface within the device.
 */
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

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the mouse HID
 *  interface within the device.
 */
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

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
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

#define ROW_NUM 5
#define COL_NUM 7

#define KEYMATRICES_NUM 2

KeyMatrix_t *leftMatrix;
KeyMatrix_t *rightMatrix;
KeyMatrix_t keyMatrices[KEYMATRICES_NUM];

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
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

    SetupHardware();

    GlobalInterruptEnable();

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

#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware()
{

    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Hardware Initialization */
    USB_Init();

    // Initialize USART
    UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
    UBRR1H = BAUD_PRESCALE >> 8;
    UBRR1L = BAUD_PRESCALE;
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);
    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Mouse_HID_Interface);
    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

    USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
    HID_Device_ProcessControlRequest(&Mouse_HID_Interface);
    HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
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
        USB_MouseReport_Data_t* MouseReport = (USB_MouseReport_Data_t*)ReportData;
        *ReportSize = sizeof(USB_MouseReport_Data_t);
        return true;
    } else if (HIDInterfaceInfo == &Generic_HID_Interface) {
        uint8_t* Data = (uint8_t*)ReportData;
        *ReportSize = GENERIC_REPORT_SIZE;
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
