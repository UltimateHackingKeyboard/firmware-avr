#ifndef PIN_H
#define PIN_H

    /* Type Defines: */
        typedef struct {
            // TODO: Investigate on why these volatile qualifiers are needed to avoid the following compiler warning:
            // warning: initialization discards ‘volatile’ qualifier from pointer target type [enabled by default]
            volatile uint8_t *Direction;
            volatile uint8_t *Name;
            uint8_t Number;
        } Pin_t;

#endif
