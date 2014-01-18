#ifndef _USB_TO_SERIAL_H_
#define _USB_TO_SERIAL_H_

    /* Includes: */
        #include <avr/io.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include <LUFA/Drivers/Peripheral/Serial.h>
        #include <LUFA/Drivers/Misc/RingBuffer.h>
        #include "USBtoSerialDescriptors.h"
        #include "Keyboard.h"

    /* Function Prototypes: */
        int USBtoSerialMainLoop(void);
        void USBtoSerialRXCallback(void);
        bool EVENT_USB_USBtoSerial_Device_ConfigurationChanged(void);
        void EVENT_USB_USBtoSerial_Device_ControlRequest(void);
        void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);
        void CatchReenumerateRequest(void);

#endif
