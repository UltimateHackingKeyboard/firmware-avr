#ifndef PORT_H
#define PORT_H

typedef struct {
    volatile uint8_t *Direction;
    volatile uint8_t *Name;
    uint8_t Number;
} Port_t;

#endif
