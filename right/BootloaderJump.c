/** \file
 *
 *  Bootloader jump functions, taken from the LUFA documentation.
 */

#include "BootloaderJump.h"

uint32_t Boot_Key ATTR_NO_INIT;

void Bootloader_Jump_Check(void)
{
    volatile uint8_t a, b, c, d, BootloaderEnumerationMode;
    __asm__ __volatile__ (
        "mov %0,r16\n"
        "mov %1,r17\n"
        "mov %2,r18\n"
        "mov %3,r19\n"
        "mov %4,r20\n"
        "clr r16\n"
        "clr r17\n"
        "clr r18\n"
        "clr r19\n"
        "clr r20\n"
        :"=r"(a), "=r"(b), "=r"(c), "=r"(d), "=r"(BootloaderEnumerationMode)
        :
        :"r16", "r17", "r18", "r19", "r20"
    );

    bool EnumerationModeSetByBootloader = a==1 && b==2 && c==3 && d==4;
    bool EnumerationModeSetByApplication = Boot_Key == MAGIC_BOOT_KEY;

    if (MCUSR & (1 << WDRF)) {
        if (EnumerationModeSetByBootloader) {
            EnumerationMode = BootloaderEnumerationMode;
        } else if (EnumerationModeSetByApplication) {
            Boot_Key = 0;
        } else {
            EnumerationMode = ENUMERATION_MODE_Keyboard;
        }
    } else {
        EnumerationMode = ENUMERATION_MODE_Keyboard;
    }

    if (EnumerationMode == ENUMERATION_MODE_Bootloader) {
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

void Reenumerate(uint8_t ReenumerateAs)
{
    EnumerationMode = ReenumerateAs;
    Jump_To_Bootloader(MAGIC_BOOT_KEY);
}
