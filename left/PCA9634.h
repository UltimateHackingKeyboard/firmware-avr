#ifndef __PCA9634_H__
#define __PCA9634_H__

    /* Includes: */
        #include "TWI.h"

    /* Macros: */
        #define PCA9634_ADDRESS 0

        // Register definitions
        #define PCA9634_REG_MODE1      0x00
        #define PCA9634_REG_MODE2      0x01
        #define PCA9634_REG_PWM0       0x02
        #define PCA9634_REG_PWM1       0x03
        #define PCA9634_REG_PWM2       0x04
        #define PCA9634_REG_PWM3       0x05
        #define PCA9634_REG_PWM4       0x06
        #define PCA9634_REG_PWM5       0x07
        #define PCA9634_REG_PWM6       0x08
        #define PCA9634_REG_PWM7       0x09
        #define PCA9634_REG_GRPPWM     0x0A
        #define PCA9634_REG_GRPFREQ    0x0B
        #define PCA9634_REG_LEDOUT0    0x0C
        #define PCA9634_REG_LEDOUT1    0x0D
        #define PCA9634_REG_SUBADR1    0x0E
        #define PCA9634_REG_SUBADR2    0x0F
        #define PCA9634_REG_SUBADR3    0x10
        #define PCA9634_REG_ALLCALLADR 0x11

        // Auto-increment options
        #define PCA9634_AI_ALL     (0b100 << 5)
        #define PCA9634_AI_PWM     (0b101 << 5)
        #define PCA9634_AI_GRP     (0b110 << 5)
        #define PCA9634_AI_PWM_GRP (0b111 << 5)

    /* Function prototypes: */
        void PCA9634_Init(void);

#endif
