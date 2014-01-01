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

#define NO_KEY 0

#include <avr/io.h>
#include "../KeyMatrix.c"
#include "UltimateHackingKeyboard.h"
#include "KeyPressMacros.h"
#include "keymaps/normalKeyMap.h"
#include "keymaps/hyperKeyMap.h"
#include "keymaps/hyperModifierMap.h"

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
{
    .Config =
        {
            .InterfaceNumber              = 0,
            .ReportINEndpoint             =
                {
                    .Address              = KEYBOARD_EPADDR,
                    .Size                 = KEYBOARD_EPSIZE,
                    .Banks                = 1,
                },
            .PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
            .PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
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
//    keyMatrices = (KeyMatrix_t*)malloc(2*sizeof(KeyMatrix_t));
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

    sei();

    KeyMatrix_Init(rightMatrix, ROW_NUM, COL_NUM);
    KeyMatrix_SetColPortsAndRowPins(rightMatrix, column_ports, row_pins);

    KeyMatrix_Init(leftMatrix, 5, 6);

    for (;;)
    {
        HID_Device_USBTask(&Keyboard_HID_Interface);
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
//    Joystick_Init();
//    LEDs_Init();
//    Buttons_Init();
    USB_Init();

    // Initialize USART
    UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
    UBRR1H = BAUD_PRESCALE >> 8;
    UBRR1L = BAUD_PRESCALE;
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
//    LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
//    LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);

    USB_Device_EnableSOFEvents();

//    LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
    HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
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
//                        if (IS_KEY_PRESSED_HYPER(keyMatrices)) {
//                            KeyboardReport->Modifier |= hyperModifierMap[row][col];
//                        }
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
/*
    uint8_t  LEDMask   = LEDS_NO_LEDS;
    uint8_t* LEDReport = (uint8_t*)ReportData;

    if (*LEDReport & HID_KEYBOARD_LED_NUMLOCK)
      LEDMask |= LEDS_LED1;

    if (*LEDReport & HID_KEYBOARD_LED_CAPSLOCK)
      LEDMask |= LEDS_LED3;

    if (*LEDReport & HID_KEYBOARD_LED_SCROLLLOCK)
      LEDMask |= LEDS_LED4;

    LEDs_SetAllLEDs(LEDMask);
*/
}
