#include <avr/io.h>
#include <util/delay.h>
#include "../KeyMatrix.c"
#include "../keycode.h"

#define ROW_NUM 5
#define COL_NUM 6

KeyMatrix_t keyMatrix;

#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void send_event(uint8_t event)
{
    while (UCSR0A & (1 << UDRE0) == 0) {};
    UDR0 = event;
}

int main(void)
{
    /* Initialize USART */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UBRR0H = BAUD_PRESCALE >> 8;
    UBRR0L = BAUD_PRESCALE;

    Port_t column_ports[COL_NUM] = {
        { .Direction=&DDRB, .Name=&PORTB, .Number=PORTB1 },
        { .Direction=&DDRB, .Name=&PORTB, .Number=PORTB0 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD7 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD6 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD5 },
        { .Direction=&DDRD, .Name=&PORTD, .Number=PORTD4 },
    };

    Pin_t row_pins[ROW_NUM] = {
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC0 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC1 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC2 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC3 },
        { .Direction=&DDRC, .Name=&PINC, .Number=PINC4 },
    };

    KeyMatrix_Init(&keyMatrix, ROW_NUM, COL_NUM);
    KeyMatrix_SetColPortsAndRowPins(&keyMatrix, column_ports, row_pins);

    while (1) {
        KeyMatrix_Scan(&keyMatrix);
        for (uint8_t row=0; row<ROW_NUM; row++) {
            for (uint8_t col=0; col<COL_NUM; col++) {
                uint8_t state = KeyMatrix_GetElement(&keyMatrix, row, col);
                if (IS_KEY_STATE_CHANGED(state)) {
                    uint8_t is_key_pressed = GET_KEY_STATE_CURRENT(state);
                    uint8_t event = EVENT_TYPE_KEY |
                                    CONSTRUCT_EVENT_STATE(is_key_pressed) |
                                    CONSTRUCT_KEYCODE(row, col, COL_NUM);
                    send_event(event);
                }
            }
        }
        _delay_ms(10);
    }
}
