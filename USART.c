#include "USART.h"

void USART_Init()
{
    UDDR |= 1 << UDDX;
    UCSRB |= (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << UCSZ0) | (1 << UCSZ1);
    UBRRH = BAUD_PRESCALE >> 8;
    UBRRL = BAUD_PRESCALE;
}

uint8_t USART_HasByte()
{
    return UCSRA & (1 << RXC);
}

uint8_t USART_ReceiveByte()
{
    return UDR;
}

void USART_SendByte(uint8_t byte)
{
    while ((UCSRA & (1 << UDRE)) == 0) {};
    UDR = byte;
}
