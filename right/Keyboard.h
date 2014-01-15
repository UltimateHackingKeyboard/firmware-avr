#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

    /* Includes: */
        #include <avr/io.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include "Config/AppConfig.h"
        #include "../KeyMatrix.h"
        #include "../USART.h"
        #include "KeyboardDescriptors.h"
        #include "KeyboardReports.h"

    /* Macros: */
        #define KEYMATRICES_NUM 2

        #define VIRUTAL_MODIFIER_TYPES_NUM 3  // Mod, Fn and Mouse

        #define KEYMAP_ID_NORMAL 0
        #define KEYMAP_ID_MOD    1
        #define KEYMAP_ID_FN     2
        #define KEYMAP_ID_MOUSE  3

        #define KEY_ACTION   0
        #define KEY_ARGUMENT 1

        #define NO_ACTION   0
        #define NO_ARGUMENT 0

        #define KEYS_NUM 65
        #define LAYOUTS_NUM (VIRUTAL_MODIFIER_TYPES_NUM + 1)
        #define ITEM_NUM_PER_KEY 2  // bytes

        // The following values must not collide with any of the HID_KEYBOARD_SC_* constants of LUFA!
        #define VIRTUAL_MODIFIER_KEY_NONE  0x00
        #define VIRTUAL_MODIFIER_KEY_MOD   0xE8
        #define VIRTUAL_MODIFIER_KEY_FN    0xE9
        #define VIRTUAL_MODIFIER_KEY_MOUSE 0xEA

    /* External Variables: */
        const extern uint8_t KeyboardLayout[ROWS_NUM][TOTAL_COLS_NUM][LAYOUTS_NUM][ITEM_NUM_PER_KEY] PROGMEM;

        extern KeyMatrix_t *leftMatrix;
        extern KeyMatrix_t *rightMatrix;
        extern KeyMatrix_t keyMatrices[KEYMATRICES_NUM];

    /* Function Prototypes: */
        int KeyboardMainLoop(void);

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
