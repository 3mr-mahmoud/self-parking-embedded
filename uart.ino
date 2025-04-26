#include "uart.h"

void uartInit(void) {
    uint16_t ubrr = F_CPU / 16 / BAUD_RATE - 1;
    
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    
    UCSR0C = (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}

void sendByte(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void sendString(const char* str) {
    while (*str) {
        sendByte(*str++);
    }
}