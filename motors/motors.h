#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>
#include "pins.h"

void motorInit(void);
void setMotorSpeed(uint8_t frontRight, uint8_t frontLeft, uint8_t rearRight, uint8_t rearLeft);
void moveForward(uint8_t speed);
void moveBackward(uint8_t speed);
void turnRight(uint8_t speed);
void turnLeft(uint8_t speed);
void turnRightForward(uint8_t speed);
void turnLeftForward(uint8_t speed);
void turnRightBackward(uint8_t speed);
void turnLeftBackward(uint8_t speed);
void rotateClockwise(uint8_t speed);
void rotateCounterClockwise(uint8_t speed);
void stopMotors(void);

#endif