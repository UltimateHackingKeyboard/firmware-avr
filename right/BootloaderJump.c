/** \file
 *
 *  Bootloader jump functions, taken from the LUFA documentation.
 */

#include "BootloaderJump.h"

wormhole_t *Wormhole ATTR_NO_INIT;

void Bootloader_Jump_Check(void)
{
    SP -= sizeof(wormhole_t);
    Wormhole = (wormhole_t*)(SP + 1);

    if ((MCUSR & (1 << WDRF)) && (Wormhole->MagicBootKey == MAGIC_BOOT_KEY)) {
        // Leave EnumerationMode as it is so that it will be processed in the application.
    } else {
        Wormhole->EnumerationMode = ENUMERATION_MODE_Keyboard;
    }
    Wormhole->MagicBootKey = 0;

    if (Wormhole->EnumerationMode == ENUMERATION_MODE_Bootloader) {
        ((void (*)(void))BOOTLOADER_START_ADDRESS)();
    }
}

void Reenumerate(uint8_t NewEnumerationMode)
{
    // If USB is used, detach from the bus and reset it
//    Delay_MS(1000);
    USB_Disable();

    // Disable all interrupts
    cli();

    // Wait two seconds for the USB detachment to register on the host
    Delay_MS(2000);

    // Set the bootloader key to the magic value and force a reset
    Wormhole->MagicBootKey = MAGIC_BOOT_KEY;
    Wormhole->EnumerationMode = NewEnumerationMode;

    wdt_enable(WDTO_250MS);
    for (;;);
}
