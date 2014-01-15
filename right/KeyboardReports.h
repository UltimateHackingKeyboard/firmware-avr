#ifndef _KEYBOARDREPORTS_H_
#define _KEYBOARDRREPORTS_H_

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
        #include "../KeyMatrix.h"
        #include "../USART.h"
        #include "Keyboard.h"
        #include "KeyboardDescriptors.h"
        #include "BootloaderJump.h"

    /* Function Prototypes: */
        bool CreateKeyboardHIDReport(void* ReportData, uint16_t* const ReportSize);
        void ProcessGenericHIDReport(const void* ReportData, const uint16_t ReportSize);

#endif
