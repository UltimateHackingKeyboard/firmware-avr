#ifndef PIN_H
#define PIN_H

typedef struct {
    volatile uint8_t *Direction;
    volatile uint8_t *Name;
    uint8_t Number;
} Pin_t;

#endif
