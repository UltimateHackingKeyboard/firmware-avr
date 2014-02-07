/** \file
 *
 *  Bootloader jump functions, taken from the LUFA documentation.
 */

#include "BootloaderJump.h"

uint32_t Boot_Key ATTR_NO_INIT;

void Bootloader_Jump_Check(void)
{
    if ((MCUSR & (1 << WDRF)) && (Boot_Key == MAGIC_BOOT_KEY)) {
        Boot_Key = 0;
    } else {
        EnumerationMode = ENUMERATION_MODE_Keyboard;
    }

    if (EnumerationMode == ENUMERATION_MODE_Bootloader) {
        ((void (*)(void))BOOTLOADER_START_ADDRESS)();
    }
}

void Reenumerate(uint8_t NewEnumerationMode)
{
    // If USB is used, detach from the bus and reset it
    USB_Disable();

    // Disable all interrupts
    cli();

    // Wait two seconds for the USB detachment to register on the host
    Delay_MS(2000);

    EnumerationMode = NewEnumerationMode;

    // Set the bootloader key to the magic value and force a reset
    Boot_Key = MAGIC_BOOT_KEY;
    wdt_enable(WDTO_250MS);
    for (;;);
}
