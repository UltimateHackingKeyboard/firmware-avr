/** \file
 *
 *  Bootloader jump functions, taken from the LUFA documentation.
 */

#include "BootloaderJump.h"

uint32_t Boot_Key ATTR_NO_INIT;

void Bootloader_Jump_Check(void)
{
    // If the reset source was the bootloader and the key is correct, clear it and jump to the bootloader
    if ((MCUSR & (1 << WDRF)) && (Boot_Key == MAGIC_BOOT_KEY)) {
        Boot_Key = 0;
        ((void (*)(void))BOOTLOADER_START_ADDRESS)();
    }
}

void Jump_To_Bootloader(uint32_t BootKey)
{
    // If USB is used, detach from the bus and reset it
    USB_Disable();

    // Disable all interrupts
    cli();

    // Wait two seconds for the USB detachment to register on the host
    Delay_MS(2000);

    // Set the bootloader key to the magic value and force a reset
    Boot_Key = BootKey;
    wdt_enable(WDTO_250MS);
    for (;;);
}
