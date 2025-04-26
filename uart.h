#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include "pins.h"

void uartInit(void);
void sendByte(uint8_t data);
void sendString(const char* str);

#endif