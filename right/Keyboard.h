#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <avr/power.h>
        #include <avr/interrupt.h>
        #include <stdbool.h>
        #include <string.h>

        #include <LUFA/Drivers/USB/USB.h>
        #include <LUFA/Platform/Platform.h>

        #include "Config/AppConfig.h"
        #include "KeyboardDescriptors.h"
        #include "KeyPressMacros.h"

    /* Function Prototypes: */
        int KeyboardMainLoop(void);
        uint8_t HasEvent(void);
        uint8_t ReadEvent(void);

        void SetupHardware(void);

        bool EVENT_USB_Keyboard_Device_ConfigurationChanged(void);
        void EVENT_USB_Keyboard_Device_ControlRequest(void);
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
