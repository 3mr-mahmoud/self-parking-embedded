#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>
#include "pins.h"

void ultrasonic_init(void);
void motors_init(void);
void move_forward(uint8_t speed);
void move_backward(uint8_t speed);
void turn_right(uint8_t speed);
void turn_left(uint8_t speed);
void rotate_right(uint8_t speed);
void rotate_right_detect(uint8_t speed);
void rotate_left(uint8_t speed);
void stop_motors(void);
void move_forward_detect(uint8_t speed);

#endif