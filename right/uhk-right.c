/** \file
 *
 *  Main file for the right keyboard half.
 */

#include "uhk-right.h"

int main(void)
{
    SetupHardware();
    GlobalInterruptEnable();

    if (Wormhole->EnumerationMode == ENUMERATION_MODE_Keyboard) {
        KeyboardMainLoop();
    } else if (Wormhole->EnumerationMode == ENUMERATION_MODE_USBtoSerial) {
        USBtoSerialMainLoop();
    }
}

void SetupHardware()
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Hardware Initialization */
    USB_Init();
}

void EVENT_USB_Device_Connect(void)
{
}

void EVENT_USB_Device_Disconnect(void)
{
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    if (Wormhole->EnumerationMode == ENUMERATION_MODE_Keyboard) {
        ConfigSuccess &= EVENT_USB_Keyboard_Device_ConfigurationChanged();
    } else if (Wormhole->EnumerationMode == ENUMERATION_MODE_USBtoSerial) {
        ConfigSuccess &= EVENT_USB_USBtoSerial_Device_ConfigurationChanged();
    }
}

void EVENT_USB_Device_ControlRequest(void)
{
    if (Wormhole->EnumerationMode == ENUMERATION_MODE_Keyboard) {
        EVENT_USB_Keyboard_Device_ControlRequest();
    } else if (Wormhole->EnumerationMode == ENUMERATION_MODE_USBtoSerial) {
        EVENT_USB_USBtoSerial_Device_ControlRequest();
    }
}

ISR(USART1_RX_vect, ISR_BLOCK)
{
    if (Wormhole->EnumerationMode == ENUMERATION_MODE_Keyboard) {
        KeyboardRxCallback();
    } else if (Wormhole->EnumerationMode == ENUMERATION_MODE_USBtoSerial) {
        USBtoSerialRxCallback();
    }
}
