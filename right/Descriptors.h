/*
             LUFA Library
     Copyright (C) Dean Camera, 2013.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

    /* Includes: */
        #include <avr/pgmspace.h>

        #include <LUFA/Drivers/USB/USB.h>

        #include "Config/AppConfig.h"
        #include "UltimateHackingKeyboard.h"
        #include "KeyboardDescriptors.h"
        #include "USBtoSerialDescriptors.h"

        /** Enum for the device interface descriptor IDs within the device. Each interface descriptor
         *  should have a unique ID index associated with it, which can be used to refer to the
         *  interface from other descriptors.
         */
        enum InterfaceDescriptors_t
        {
            INTERFACE_ID_Keyboard   = 0, /**< Keyboard interface descriptor ID */
            INTERFACE_ID_Mouse      = 1, /**< Mouse interface descriptor ID */
            INTERFACE_ID_GenericHID = 2, /**< GenericHID interface descriptor ID */

            INTERFACE_ID_CDC_CCI = 0, /**< CDC CCI interface descriptor ID */
            INTERFACE_ID_CDC_DCI = 1, /**< CDC DCI interface descriptor ID */
        };

    /* Function Prototypes: */
        uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                            const uint8_t wIndex,
                                            const void** const DescriptorAddress)
                                            ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif

