#ifndef _BOOTLOADER_JUMP_H_
#define _BOOTLOADER_JUMP_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <LUFA/Drivers/USB/USB.h>

    /* Macros: */
        #define MAGIC_BOOT_KEY            0xDC42ACCA
        #define MAGIC_USB_TO_SERIAL_KEY   0x34A9B68C
        #define FLASH_SIZE_BYTES          32768
        #define BOOTLOADER_SEC_SIZE_BYTES 4096
        #define BOOTLOADER_START_ADDRESS  (FLASH_SIZE_BYTES - BOOTLOADER_SEC_SIZE_BYTES)

        #define REENUMERATE_AS_KEYBOARD      0
        #define REENUMERATE_AS_BOOTLOADER    1
        #define REENUMERATE_AS_USB_TO_SERIAL 2

    /* Global Variables: */
        extern uint32_t Boot_Key ATTR_NO_INIT;

    /* Function Prototypes: */
        void Bootloader_Jump_Check(void) ATTR_INIT_SECTION(3);
        void Jump_To_Bootloader(uint32_t BootKey);
        void Reenumerate(uint8_t ReenumerateAs);

#endif
