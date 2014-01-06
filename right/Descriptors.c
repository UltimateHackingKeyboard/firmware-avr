/** \file
 *
 *  Main file for USB Device Descriptors.
 */

#include "Descriptors.h"

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress)
{
    if (EnumerationMode == ENUMERATION_MODE_Keyboard) {
            return CALLBACK_USB_GetKeyboardDescriptor(wValue, wIndex, DescriptorAddress);
    } else {  // EnumerationMode == ENUMERATION_MODE_USBtoSerial
            return CALLBACK_USB_GetUSBtoSerialDescriptor(wValue, wIndex, DescriptorAddress);
    }
}
