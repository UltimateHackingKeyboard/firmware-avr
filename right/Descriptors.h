#ifndef __DESCRIPTORS_H__
#define __DESCRIPTORS_H__

    /* Includes: */
        #include <LUFA/Drivers/USB/USB.h>
        #include "uhk-right.h"

    /* Macros: */
        // TODO: Restore the VID to 0x16D0 for the final prototype.
        #define VID 0x16D1

        #define PID_KEYBOARD_6KRO 0x05EA
        #define PID_KEYBOARD_NKRO 0x05EB
        #define PID_BOOTLOADER    0x05EC
        #define PID_USB_TO_SERIAL 0x05ED

    /* Function Prototypes: */
        uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                            const uint8_t wIndex,
                                            const void** const DescriptorAddress)
                                            ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
