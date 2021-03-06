#ifndef __UHK_RIGHT_H__
#define __UHK_RIGHT_H__

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <avr/power.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include "Keyboard.h"
        #include "USBtoSerial.h"
        #include "BootloaderJump.h"
        #include "TWI.h"

    /* Type Defines: */
        /** Enum for the USB enumeration mode. */
        enum EnumerationMode_t
        {
            ENUMERATION_MODE_Keyboard    = 0, /**< Normal enumeration mode featuring a keyboard, a mouse and a generic HID interface */
            ENUMERATION_MODE_Bootloader  = 1, /**< Bootloader enumeration mode for upgrading the firmware of the right half */
            ENUMERATION_MODE_USBtoSerial = 2, /**< USBtoSerial enumeration mode for upgrading the firmware of the left half */
        };

    /* Function Prototypes: */
        int KeyboardMainLoop(void);
        void SetupHardware(void);

        void EVENT_USB_Device_Connect(void);
        void EVENT_USB_Device_Disconnect(void);
        void EVENT_USB_Device_ConfigurationChanged(void);
        void EVENT_USB_Device_ControlRequest(void);
        void EVENT_USB_Device_StartOfFrame(void);

        bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                                 uint8_t* const ReportID,
                                                 const uint8_t ReportType,
                                                 void* ReportData,
                                                 uint16_t* const ReportSize);

        void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                                  const uint8_t ReportID,
                                                  const uint8_t ReportType,
                                                  const void* ReportData,
                                                  const uint16_t ReportSize);

#endif
