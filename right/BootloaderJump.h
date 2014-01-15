#ifndef _BOOTLOADER_JUMP_H_
#define _BOOTLOADER_JUMP_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <LUFA/Drivers/USB/USB.h>

    /* Macros: */
        #define MAGIC_BOOT_KEY            0xDC42ACCA
        #define FLASH_SIZE_BYTES          32768
        #define BOOTLOADER_SEC_SIZE_BYTES 4096
        #define BOOTLOADER_START_ADDRESS  (FLASH_SIZE_BYTES - BOOTLOADER_SEC_SIZE_BYTES)

    /* Function Prototypes: */
        void Bootloader_Jump_Check(void) ATTR_INIT_SECTION(3);
        void Jump_To_Bootloader(void);

#endif
