#include "USART.h"

void USART_Init()
{
    UCSRB |= (1 << RXEN) | (1 << TXEN);
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
