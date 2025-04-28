#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#include "pins.h"

void timer1_init(void);
uint16_t timer1_read_us(void);
uint16_t ultrasonic_read(uint8_t trig_bit, uint8_t echo_bit);
void ultrasonic_init(void);

#endif // ULTRASONIC_H