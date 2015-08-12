#ifndef __KEYBOARD_DESCRIPTORS_H__
#define __KEYBOARD_DESCRIPTORS_H__

    /* Includes: */
        #include <LUFA/Drivers/USB/USB.h>

    /* Type Defines: */
        typedef struct
        {
            USB_Descriptor_Configuration_Header_t Config;

            // Generic HID Interface
            USB_Descriptor_Interface_t            HID1_GenericHIDInterface;
            USB_HID_Descriptor_HID_t              HID1_GenericHID;
            USB_Descriptor_Endpoint_t             HID1_ReportINEndpoint;

            // Keyboard HID Interface
            USB_Descriptor_Interface_t            HID2_KeyboardInterface;
            USB_HID_Descriptor_HID_t              HID2_KeyboardHID;
            USB_Descriptor_Endpoint_t             HID2_ReportINEndpoint;

            // Mouse HID Interface
            USB_Descriptor_Interface_t            HID3_MouseInterface;
            USB_HID_Descriptor_HID_t              HID3_MouseHID;
            USB_Descriptor_Endpoint_t             HID3_ReportINEndpoint;
        } Keyboard_USB_Descriptor_Configuration_t;

        typedef struct
        {
            uint8_t Button; /**< Button mask for currently pressed buttons in the mouse. */
            int8_t  X; /**< Current delta X movement of the mouse. */
            int8_t  Y; /**< Current delta Y movement on the mouse. */
            int8_t  VerticalWheelMovement;
            int8_t  HorizontalWheelMovement;
        } ATTR_PACKED USB_WheeledMouseReport_Data_t;

        enum KeyboardInterfaceDescriptors_t
        {
            INTERFACE_ID_GenericHID = 0, /**< Generic HID interface descriptor ID */
            INTERFACE_ID_Keyboard   = 1, /**< Keyboard interface descriptor ID */
            INTERFACE_ID_Mouse      = 2, /**< Mouse interface descriptor ID */
        };

        enum KeyboardStringDescriptors_t
        {
            STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
            STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
            STRING_ID_Product      = 2, /**< Product string ID */
        };

    /* Macros: */
        /** Endpoint address of the Keyboard HID reporting IN endpoint. */
        #define KEYBOARD_IN_EPADDR        (ENDPOINT_DIR_IN | 1)

        #define KEYBOARD_ROLLOVER         6  // key

        /** Endpoint address of the Mouse HID reporting IN endpoint. */
        #define MOUSE_IN_EPADDR           (ENDPOINT_DIR_IN | 2)

        /** Size in bytes of each of the HID reporting IN endpoints. */
        #define HID_EPSIZE                8

        /** Endpoint address of the Generic HID reporting IN endpoint. */
        #define GENERIC_IN_EPADDR         (ENDPOINT_DIR_IN | 3)

        /** Size in bytes of the Generic HID reporting endpoint. */
        #define GENERIC_EPSIZE            64

        #define HID_DESCRIPTOR_WHEELED_MOUSE(MinAxisVal, MaxAxisVal, MinPhysicalVal, MaxPhysicalVal, Buttons, AbsoluteCoords) \
            HID_RI_USAGE_PAGE(8, 0x01),                     \
            HID_RI_USAGE(8, 0x02),                          \
            HID_RI_COLLECTION(8, 0x01),                     \
                HID_RI_USAGE(8, 0x01),                      \
                HID_RI_COLLECTION(8, 0x00),                 \
                    HID_RI_USAGE_PAGE(8, 0x09),             \
                    HID_RI_USAGE_MINIMUM(8, 0x01),          \
                    HID_RI_USAGE_MAXIMUM(8, Buttons),       \
                    HID_RI_LOGICAL_MINIMUM(8, 0x00),        \
                    HID_RI_LOGICAL_MAXIMUM(8, 0x01),        \
                    HID_RI_REPORT_COUNT(8, Buttons),        \
                    HID_RI_REPORT_SIZE(8, 0x01),            \
                    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE), \
                    HID_RI_REPORT_COUNT(8, 0x01),           \
                    HID_RI_REPORT_SIZE(8, (Buttons % 8) ? (8 - (Buttons % 8)) : 0), \
                    HID_RI_INPUT(8, HID_IOF_CONSTANT),      \
                    HID_RI_USAGE_PAGE(8, 0x01),             \
                    HID_RI_USAGE(8, 0x30),                  \
                    HID_RI_USAGE(8, 0x31),                  \
                    HID_RI_LOGICAL_MINIMUM(16, MinAxisVal), \
                    HID_RI_LOGICAL_MAXIMUM(16, MaxAxisVal), \
                    HID_RI_PHYSICAL_MINIMUM(16, MinPhysicalVal), \
                    HID_RI_PHYSICAL_MAXIMUM(16, MaxPhysicalVal), \
                    HID_RI_REPORT_COUNT(8, 0x02),           \
                    HID_RI_REPORT_SIZE(8, (((MinAxisVal >= -128) && (MaxAxisVal <= 127)) ? 8 : 16)), \
                    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | (AbsoluteCoords ? HID_IOF_ABSOLUTE : HID_IOF_RELATIVE)), \
                    0xa1, 0x02,        /*       COLLECTION (Logical) */ \
                                       /* ------------------------------  Vertical wheel res multiplier */ \
                    0x09, 0x48,        /*         USAGE (Resolution Multiplier) */ \
                    0x15, 0x00,        /*         LOGICAL_MINIMUM (0) */ \
                    0x25, 0x01,        /*         LOGICAL_MAXIMUM (1) */ \
                    0x35, 0x01,        /*         PHYSICAL_MINIMUM (1) */ \
                    0x45, 0x04,        /*         PHYSICAL_MAXIMUM (4) */ \
                    0x75, 0x02,        /*         REPORT_SIZE (2) */ \
                    0x95, 0x01,        /*         REPORT_COUNT (1) */ \
                    0xa4,              /*         PUSH */ \
                    0xb1, 0x02,        /*         FEATURE (Data,Var,Abs) */ \
                                       /* ------------------------------  Vertical wheel */ \
                    0x09, 0x38,        /*         USAGE (Wheel) */ \
                    0x15, 0x81,        /*         LOGICAL_MINIMUM (-127) */ \
                    0x25, 0x7f,        /*         LOGICAL_MAXIMUM (127) */ \
                    0x35, 0x00,        /*         PHYSICAL_MINIMUM (0)        - reset physical */ \
                    0x45, 0x00,        /*         PHYSICAL_MAXIMUM (0) */ \
                    0x75, 0x08,        /*         REPORT_SIZE (8) */ \
                    0x81, 0x06,        /*         INPUT (Data,Var,Rel) */ \
                    0xc0,              /*       END_COLLECTION */ \
                    0xa1, 0x02,        /*       COLLECTION (Logical) */ \
                                       /* ------------------------------  Horizontal wheel res multiplier */ \
                    0x09, 0x48,        /*         USAGE (Resolution Multiplier) */ \
                    0xb4,              /*         POP */ \
                    0xb1, 0x02,        /*         FEATURE (Data,Var,Abs) */ \
                                       /* ------------------------------  Padding for Feature report */ \
                    0x35, 0x00,        /*         PHYSICAL_MINIMUM (0)        - reset physical */ \
                    0x45, 0x00,        /*         PHYSICAL_MAXIMUM (0) */ \
                    0x75, 0x04,        /*         REPORT_SIZE (4) */ \
                    0xb1, 0x03,        /*         FEATURE (Cnst,Var,Abs) */ \
                                       /* ------------------------------  Horizontal wheel */ \
                    0x05, 0x0c,        /*         USAGE_PAGE (Consumer Devices) */ \
                    0x0a, 0x38, 0x02,  /*         USAGE (AC Pan) */ \
                    0x15, 0x81,        /*         LOGICAL_MINIMUM (-127) */ \
                    0x25, 0x7f,        /*         LOGICAL_MAXIMUM (127) */ \
                    0x75, 0x08,        /*         REPORT_SIZE (8) */ \
                    0x81, 0x06,        /*         INPUT (Data,Var,Rel) */ \
                    0xc0,              /*       END_COLLECTION */ \
                HID_RI_END_COLLECTION(0),                   \
            HID_RI_END_COLLECTION(0)

    /* Function Prototypes: */
        uint16_t CALLBACK_USB_GetKeyboardDescriptor(const uint16_t wValue,
                                                    const uint8_t wIndex,
                                                    const void** const DescriptorAddress);

#endif
