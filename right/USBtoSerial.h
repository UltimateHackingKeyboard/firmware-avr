/** \file
 *
 *  Header file for USBtoSerial.c.
 */

#ifndef _USBTOSERIAL_H_
#define _USBTOSERIAL_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <avr/power.h>
        #include <avr/interrupt.h>
        #include <stdbool.h>
        #include <string.h>

        #include "Descriptors.h"

        #include <LUFA/Drivers/Peripheral/Serial.h>
        #include <LUFA/Drivers/Misc/RingBuffer.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include <LUFA/Platform/Platform.h>

    /* Function Prototypes: */
        int USBtoSerialMainLoop(void);
        void USBtoSerialRXCallback(void);
        bool EVENT_USB_USBtoSerial_Device_ConfigurationChanged(void);
        void EVENT_USB_USBtoSerial_Device_ControlRequest(void);
        void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);

#endif
