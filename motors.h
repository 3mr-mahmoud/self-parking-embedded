#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>
#include "pins.h"

void motorInit(void);
void setRightMotorSpeed(uint8_t speed);
void setLeftMotorSpeed(uint8_t speed);
void moveForward(uint8_t speed);
void moveBackward(uint8_t speed);
void turnRight(uint8_t speed);
void turnLeft(uint8_t speed);
void stopMotors(void);

#endif