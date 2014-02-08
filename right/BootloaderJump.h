#ifndef _BOOTLOADER_JUMP_H_
#define _BOOTLOADER_JUMP_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include "uhk-right.h"

    /* Macros: */
        #define MAGIC_BOOT_KEY            0xDC42ACCA
        #define FLASH_SIZE_BYTES          32768
        #define BOOTLOADER_SEC_SIZE_BYTES 4096
        #define BOOTLOADER_START_ADDRESS  (FLASH_SIZE_BYTES - BOOTLOADER_SEC_SIZE_BYTES)

    /* Type Defines: */
        typedef struct {
          uint32_t MagicBootKey;
          uint8_t EnumerationMode;
        } wormhole_t;

    /* Global Variables: */
        extern wormhole_t *Wormhole ATTR_NO_INIT;

    /* Function Prototypes: */
        void Bootloader_Jump_Check(void) ATTR_INIT_SECTION(3);
        void Reenumerate(uint8_t NewEnumerationMode);

#endif
