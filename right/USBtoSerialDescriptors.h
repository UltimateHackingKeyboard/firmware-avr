#ifndef _USB_TO_SERIAL_DESCRIPTORS_H_
#define _USB_TO_SERIAL_DESCRIPTORS_H_

    /* Includes: */
        #include <LUFA/Drivers/USB/USB.h>
        #include "Descriptors.h"

    /* Type Defines: */
        typedef struct
        {
            USB_Descriptor_Configuration_Header_t    Config;

            // CDC Command Interface
            USB_Descriptor_Interface_t               CDC_CCI_Interface;
            USB_CDC_Descriptor_FunctionalHeader_t    CDC_Functional_Header;
            USB_CDC_Descriptor_FunctionalACM_t       CDC_Functional_ACM;
            USB_CDC_Descriptor_FunctionalUnion_t     CDC_Functional_Union;
            USB_Descriptor_Endpoint_t                CDC_NotificationEndpoint;

            // CDC Data Interface
            USB_Descriptor_Interface_t               CDC_DCI_Interface;
            USB_Descriptor_Endpoint_t                CDC_DataOutEndpoint;
            USB_Descriptor_Endpoint_t                CDC_DataInEndpoint;
        } USBtoSerial_USB_Descriptor_Configuration_t;

        enum USBtoSerialInterfaceDescriptors_t
        {
            INTERFACE_ID_CDC_CCI = 0, /**< CDC CCI interface descriptor ID */
            INTERFACE_ID_CDC_DCI = 1, /**< CDC DCI interface descriptor ID */
        };

        enum USBtoSerialStringDescriptors_t
        {
            STRING_ID_USBtoSerial_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
            STRING_ID_USBtoSerial_Manufacturer = 1, /**< Manufacturer string ID */
            STRING_ID_USBtoSerial_Product      = 2, /**< Product string ID */
        };

    /* Macros: */
        /** Endpoint address of the CDC device-to-host notification IN endpoint. */
        #define CDC_NOTIFICATION_EPADDR        (ENDPOINT_DIR_IN  | 2)

        /** Endpoint address of the CDC device-to-host data IN endpoint. */
        #define CDC_TX_EPADDR                  (ENDPOINT_DIR_IN  | 3)

        /** Endpoint address of the CDC host-to-device data OUT endpoint. */
        #define CDC_RX_EPADDR                  (ENDPOINT_DIR_OUT | 4)

        /** Size in bytes of the CDC device-to-host notification IN endpoint. */
        #define CDC_NOTIFICATION_EPSIZE        8

        /** Size in bytes of the CDC data IN and OUT endpoints. */
        #define CDC_TXRX_EPSIZE                16

    /* Function Prototypes: */
        uint16_t CALLBACK_USB_GetUSBtoSerialDescriptor(const uint16_t wValue,
                                                       const uint8_t wIndex,
                                                       const void** const DescriptorAddress)
                                                       ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
