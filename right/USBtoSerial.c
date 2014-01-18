/** \file
 *
 *  Main file for the USB to Serial function of the keyboard, taken from LUFA.
 *  It's used for USART-programming the left keyboard half via the right keyboard half.
 */

#include "USBtoSerial.h"

/** Circular buffer to hold data from the host before it is sent to the device via the serial port. */
static RingBuffer_t USBtoUSART_Buffer;

/** Underlying data buffer for \ref USBtoUSART_Buffer, where the stored bytes are located. */
static uint8_t      USBtoUSART_Buffer_Data[128];

/** Circular buffer to hold data from the serial port before it is sent to the host. */
static RingBuffer_t USARTtoUSB_Buffer;

/** Underlying data buffer for \ref USARTtoUSB_Buffer, where the stored bytes are located. */
static uint8_t      USARTtoUSB_Buffer_Data[128];

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
    {
        .Config =
            {
                .ControlInterfaceNumber         = INTERFACE_ID_CDC_CCI,
                .DataINEndpoint                 =
                    {
                        .Address                = CDC_TX_EPADDR,
                        .Size                   = CDC_TXRX_EPSIZE,
                        .Banks                  = 1,
                    },
                .DataOUTEndpoint                =
                    {
                        .Address                = CDC_RX_EPADDR,
                        .Size                   = CDC_TXRX_EPSIZE,
                        .Banks                  = 1,
                    },
                .NotificationEndpoint           =
                    {
                        .Address                = CDC_NOTIFICATION_EPADDR,
                        .Size                   = CDC_NOTIFICATION_EPSIZE,
                        .Banks                  = 1,
                    },
            },
    };

int USBtoSerialMainLoop(void)
{
    RingBuffer_InitBuffer(&USBtoUSART_Buffer, USBtoUSART_Buffer_Data, sizeof(USBtoUSART_Buffer_Data));
    RingBuffer_InitBuffer(&USARTtoUSB_Buffer, USARTtoUSB_Buffer_Data, sizeof(USARTtoUSB_Buffer_Data));

    for (;;)
    {
        /* Only try to read in bytes from the CDC interface if the transmit buffer is not full */
        if (!(RingBuffer_IsFull(&USBtoUSART_Buffer)))
        {
            int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

            /* Store received byte into the USART transmit buffer */
            if (!(ReceivedByte < 0))
              RingBuffer_Insert(&USBtoUSART_Buffer, ReceivedByte);
        }

        uint16_t BufferCount = RingBuffer_GetCount(&USARTtoUSB_Buffer);
        if (BufferCount)
        {
            Endpoint_SelectEndpoint(VirtualSerial_CDC_Interface.Config.DataINEndpoint.Address);

            /* Check if a packet is already enqueued to the host - if so, we shouldn't try to send more data
             * until it completes as there is a chance nothing is listening and a lengthy timeout could occur */
            if (Endpoint_IsINReady())
            {
                /* Never send more than one bank size less one byte to the host at a time, so that we don't block
                 * while a Zero Length Packet (ZLP) to terminate the transfer is sent if the host isn't listening */
                uint8_t BytesToSend = MIN(BufferCount, (CDC_TXRX_EPSIZE - 1));

                /* Read bytes from the USART receive buffer into the USB IN endpoint */
                while (BytesToSend--)
                {
                    /* Try to send the next byte of data to the host, abort if there is an error without dequeuing */
                    if (CDC_Device_SendByte(&VirtualSerial_CDC_Interface,
                                            RingBuffer_Peek(&USARTtoUSB_Buffer)) != ENDPOINT_READYWAIT_NoError)
                    {
                        break;
                    }

                    /* Dequeue the already sent byte from the buffer now we have confirmed that no transmission error occurred */
                    RingBuffer_Remove(&USARTtoUSB_Buffer);
                }
            }
        }

        /* Load the next byte from the USART transmit buffer into the USART */
        if (!(RingBuffer_IsEmpty(&USBtoUSART_Buffer)))
          Serial_SendByte(RingBuffer_Remove(&USBtoUSART_Buffer));

        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}

void USBtoSerialRXCallback()
{
    uint8_t ReceivedByte = UDR1;

    if (USB_DeviceState == DEVICE_STATE_Configured)
      RingBuffer_Insert(&USARTtoUSB_Buffer, ReceivedByte);
}

bool EVENT_USB_USBtoSerial_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;
    ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
    return ConfigSuccess;
}

void EVENT_USB_USBtoSerial_Device_ControlRequest(void)
{
    CatchReenumerateRequest();
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
    uint8_t ConfigMask = 0;

    switch (CDCInterfaceInfo->State.LineEncoding.ParityType)
    {
        case CDC_PARITY_Odd:
            ConfigMask = ((1 << UPM11) | (1 << UPM10));
            break;
        case CDC_PARITY_Even:
            ConfigMask = (1 << UPM11);
            break;
    }

    if (CDCInterfaceInfo->State.LineEncoding.CharFormat == CDC_LINEENCODING_TwoStopBits)
      ConfigMask |= (1 << USBS1);

    switch (CDCInterfaceInfo->State.LineEncoding.DataBits)
    {
        case 6:
            ConfigMask |= (1 << UCSZ10);
            break;
        case 7:
            ConfigMask |= (1 << UCSZ11);
            break;
        case 8:
            ConfigMask |= ((1 << UCSZ11) | (1 << UCSZ10));
            break;
    }

    /* Must turn off USART before reconfiguring it, otherwise incorrect operation may occur */
    UCSR1B = 0;
    UCSR1A = 0;
    UCSR1C = 0;

    /* Set the new baud rate before configuring the USART */
    UBRR1  = SERIAL_2X_UBBRVAL(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);

    /* Reconfigure the USART in double speed mode for a wider baud rate range at the expense of accuracy */
    UCSR1C = ConfigMask;
    UCSR1A = (1 << U2X1);
    UCSR1B = ((1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));
}

void CatchReenumerateRequest()
{
    if (!(Endpoint_IsSETUPReceived())) {
        return;
    }

    if (USB_ControlRequest.wIndex != INTERFACE_ID_CDC_CCI) {
        return;
    }

    if (USB_ControlRequest.bRequest != HID_REQ_SetReport) {
        return;
    }

    if (USB_ControlRequest.bmRequestType != (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) {
        return;
    }

    uint16_t ReportSize = USB_ControlRequest.wLength;
    uint8_t  ReportData[ReportSize];

    Endpoint_ClearSETUP();
    Endpoint_Read_Control_Stream_LE(ReportData, ReportSize);
    Endpoint_ClearIN();

    uint8_t Command = ReportData[0];
    if (Command != AGENT_COMMAND_REENUMERATE) {
        return;
    }

    uint8_t ReenumerateAs = ReportData[1];
    Reenumerate(ReenumerateAs);
}
