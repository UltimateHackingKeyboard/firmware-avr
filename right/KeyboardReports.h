#ifndef _KEYBOARD_REPORTS_H_
#define _KEYBOARD_REPORTS_H_

    /* Includes: */
        #include <avr/io.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include "Keyboard.h"

    /* Function Prototypes: */
        bool CreateKeyboardHIDReport(void* ReportData, uint16_t* const ReportSize);
        bool CreateMouseHIDReport(void* ReportData, uint16_t* const ReportSize);
        bool CreateGenericHIDReport(void* ReportData, uint16_t* const ReportSize);
        void ProcessGenericHIDReport(const void* ReportData, const uint16_t ReportSize);

#endif
