/** \file
 *
 *  USB Device Descriptors for the keyboard.
 */

#include "Descriptors.h"

const USB_Descriptor_HIDReport_Datatype_t PROGMEM KeyboardReport[] =
{
    /* Use the HID class driver's standard Keyboard report.
     *   Max simultaneous keys: KEYBOARD_ROLLOVER
     */
    HID_DESCRIPTOR_KEYBOARD(KEYBOARD_ROLLOVER)
};

// TODO: Enlarge axis values as much as possible both here and in LUFA.
const USB_Descriptor_HIDReport_Datatype_t PROGMEM MouseReport[] =
{
    /* Use the HID class driver's standard Mouse report.
     *   Min X/Y Axis values: -1
     *   Max X/Y Axis values:  1
     *   Min physical X/Y Axis values (used to determine resolution): -1
     *   Max physical X/Y Axis values (used to determine resolution):  1
     *   Buttons: 3
     *   Absolute screen coordinates: false
     */
    HID_DESCRIPTOR_WHEELED_MOUSE(-127, 127, -127, 127, 3, false)
};

const USB_Descriptor_HIDReport_Datatype_t PROGMEM GenericReport[] =
{
    /* Use the HID class driver's standard Vendor HID report.
     *  Vendor Usage Page: 0
     *  Vendor Collection Usage: 1
     *  Vendor Report IN Usage: 2
     *  Vendor Report OUT Usage: 3
     *  Vendor Report Size: GENERIC_REPORT_SIZE
     */
    HID_DESCRIPTOR_VENDOR(0x00, 0x01, 0x02, 0x03, GENERIC_REPORT_SIZE)
};

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
    .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

    .USBSpecification       = VERSION_BCD(1, 1, 0),
    .Class                  = USB_CSCP_NoDeviceClass,
    .SubClass               = USB_CSCP_NoDeviceSubclass,
    .Protocol               = USB_CSCP_NoDeviceProtocol,

    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

    .VendorID               = VID,
    .ProductID              = PID_KEYBOARD_6KRO,
    .ReleaseNumber          = VERSION_BCD(0, 0, 1),

    .ManufacturerStrIndex   = STRING_ID_Manufacturer,
    .ProductStrIndex        = STRING_ID_Product,
    .SerialNumStrIndex      = NO_DESCRIPTOR,

    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const Keyboard_USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
    .Config =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

            .TotalConfigurationSize = sizeof(Keyboard_USB_Descriptor_Configuration_t),
            .TotalInterfaces        = 3,

            .ConfigurationNumber    = 1,
            .ConfigurationStrIndex  = NO_DESCRIPTOR,

            .ConfigAttributes       = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

            .MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
        },

    .HID1_GenericHIDInterface =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

            .InterfaceNumber        = INTERFACE_ID_GenericHID,
            .AlternateSetting       = 0x00,

            .TotalEndpoints         = 1,

            .Class                  = HID_CSCP_HIDClass,
            .SubClass               = HID_CSCP_NonBootSubclass,
            .Protocol               = HID_CSCP_NonBootProtocol,

            .InterfaceStrIndex      = NO_DESCRIPTOR
        },

    .HID1_GenericHID =
        {
            .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

            .HIDSpec                = VERSION_BCD(1, 1, 1),
            .CountryCode            = 0x00,
            .TotalReportDescriptors = 1,
            .HIDReportType          = HID_DTYPE_Report,
            .HIDReportLength        = sizeof(GenericReport)
        },

    .HID1_ReportINEndpoint =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

            .EndpointAddress        = GENERIC_IN_EPADDR,
            .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
            .EndpointSize           = GENERIC_EPSIZE,
            .PollingIntervalMS      = 0x05
        },

    .HID2_KeyboardInterface =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

            .InterfaceNumber        = INTERFACE_ID_Keyboard,
            .AlternateSetting       = 0x00,

            .TotalEndpoints         = 1,

            .Class                  = HID_CSCP_HIDClass,
            .SubClass               = HID_CSCP_BootSubclass,
            .Protocol               = HID_CSCP_KeyboardBootProtocol,

            .InterfaceStrIndex      = NO_DESCRIPTOR
        },

    .HID2_KeyboardHID =
        {
            .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

            .HIDSpec                = VERSION_BCD(1, 1, 1),
            .CountryCode            = 0x00,
            .TotalReportDescriptors = 1,
            .HIDReportType          = HID_DTYPE_Report,
            .HIDReportLength        = sizeof(KeyboardReport)
        },

    .HID2_ReportINEndpoint =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

            .EndpointAddress        = KEYBOARD_IN_EPADDR,
            .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
            .EndpointSize           = HID_EPSIZE,
            .PollingIntervalMS      = 0x05
        },

    .HID3_MouseInterface =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

            .InterfaceNumber        = INTERFACE_ID_Mouse,
            .AlternateSetting       = 0x00,

            .TotalEndpoints         = 1,

            .Class                  = HID_CSCP_HIDClass,
            .SubClass               = HID_CSCP_BootSubclass,
            .Protocol               = HID_CSCP_MouseBootProtocol,

            .InterfaceStrIndex      = NO_DESCRIPTOR
        },

    .HID3_MouseHID =
        {
            .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

            .HIDSpec                = VERSION_BCD(1, 1, 1),
            .CountryCode            = 0x00,
            .TotalReportDescriptors = 1,
            .HIDReportType          = HID_DTYPE_Report,
            .HIDReportLength        = sizeof(MouseReport)
        },

    .HID3_ReportINEndpoint =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

            .EndpointAddress        = MOUSE_IN_EPADDR,
            .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
            .EndpointSize           = HID_EPSIZE,
            .PollingIntervalMS      = 0x05
        }
};

const USB_Descriptor_String_t PROGMEM LanguageString =
{
    .Header                 = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},

    .UnicodeString          = {LANGUAGE_ID_ENG}
};

const USB_Descriptor_String_t PROGMEM ManufacturerString =
{
    .Header                 = {.Size = USB_STRING_LEN(28), .Type = DTYPE_String},

    .UnicodeString          = L"Ultimate Gadget Laboratories"
};

const USB_Descriptor_String_t PROGMEM ProductString =
{
    .Header                 = {.Size = USB_STRING_LEN(37), .Type = DTYPE_String},

    .UnicodeString          = L"Ultimate Hacking Keyboard (6KRO mode)"
};

uint16_t CALLBACK_USB_GetKeyboardDescriptor(const uint16_t wValue,
                                            const uint8_t wIndex,
                                            const void** const DescriptorAddress)
{
    const uint8_t  DescriptorType   = (wValue >> 8);
    const uint8_t  DescriptorNumber = (wValue & 0xFF);

    const void* Address = NULL;
    uint16_t    Size    = NO_DESCRIPTOR;

    switch (DescriptorType)
    {
        case DTYPE_Device:
            Address = &DeviceDescriptor;
            Size    = sizeof(USB_Descriptor_Device_t);
            break;
        case DTYPE_Configuration:
            Address = &ConfigurationDescriptor;
            Size    = sizeof(Keyboard_USB_Descriptor_Configuration_t);
            break;
        case DTYPE_String:
            switch (DescriptorNumber)
            {
                case STRING_ID_Language:
                    Address = &LanguageString;
                    Size    = pgm_read_byte(&LanguageString.Header.Size);
                    break;
                case STRING_ID_Manufacturer:
                    Address = &ManufacturerString;
                    Size    = pgm_read_byte(&ManufacturerString.Header.Size);
                    break;
                case STRING_ID_Product:
                    Address = &ProductString;
                    Size    = pgm_read_byte(&ProductString.Header.Size);
                    break;
            }

            break;
        case HID_DTYPE_HID:
            switch (wIndex)
            {
                case INTERFACE_ID_GenericHID:
                    Address = &ConfigurationDescriptor.HID1_GenericHID;
                    Size    = sizeof(USB_HID_Descriptor_HID_t);
                    break;
                case INTERFACE_ID_Keyboard:
                    Address = &ConfigurationDescriptor.HID2_KeyboardHID;
                    Size    = sizeof(USB_HID_Descriptor_HID_t);
                    break;
                case INTERFACE_ID_Mouse:
                    Address = &ConfigurationDescriptor.HID3_MouseHID;
                    Size    = sizeof(USB_HID_Descriptor_HID_t);
                    break;
            }

            break;
        case HID_DTYPE_Report:
            switch (wIndex)
            {
                case INTERFACE_ID_GenericHID:
                    Address = &GenericReport;
                    Size    = sizeof(GenericReport);
                    break;
                case INTERFACE_ID_Keyboard:
                    Address = &KeyboardReport;
                    Size    = sizeof(KeyboardReport);
                    break;
                case INTERFACE_ID_Mouse:
                    Address = &MouseReport;
                    Size    = sizeof(MouseReport);
                    break;
            }

            break;
    }

    *DescriptorAddress = Address;
    return Size;
}
