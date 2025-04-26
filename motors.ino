#include "motors.h"
#include "pins.h"

void motorInit(void) {
    MOTOR_RIGHT_IN1_DDR |= (1 << MOTOR_RIGHT_IN1_PIN);
    MOTOR_RIGHT_IN2_DDR |= (1 << MOTOR_RIGHT_IN2_PIN);
    MOTOR_LEFT_IN3_DDR |= (1 << MOTOR_LEFT_IN3_PIN);
    MOTOR_LEFT_IN4_DDR |= (1 << MOTOR_LEFT_IN4_PIN);
    MOTOR_RIGHT_ENA_DDR |= (1 << MOTOR_RIGHT_ENA_PIN);
    MOTOR_LEFT_ENB_DDR |= (1 << MOTOR_LEFT_ENB_PIN);
    
    TCCR1A = (1 << WGM11) | (0 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << WGM12);
    
    TCCR1A |= (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0);
    
    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
    
    ICR1 = 40000;
    
    OCR1A = 0;
    OCR1B = 0;
}

void setRightMotorSpeed(uint8_t speed) {
    OCR1A = (uint16_t)speed * (ICR1 / 255);
}

void setLeftMotorSpeed(uint8_t speed) {
    OCR1B = (uint16_t)speed * (ICR1 / 255);
}

void moveForward(uint8_t speed) {
    MOTOR_RIGHT_IN1_PORT |= (1 << MOTOR_RIGHT_IN1_PIN);
    MOTOR_RIGHT_IN2_PORT &= ~(1 << MOTOR_RIGHT_IN2_PIN);
    
    MOTOR_LEFT_IN3_PORT |= (1 << MOTOR_LEFT_IN3_PIN);
    MOTOR_LEFT_IN4_PORT &= ~(1 << MOTOR_LEFT_IN4_PIN);
    
    setRightMotorSpeed(speed);
    setLeftMotorSpeed(speed);
}

void moveBackward(uint8_t speed) {
    MOTOR_RIGHT_IN1_PORT &= ~(1 << MOTOR_RIGHT_IN1_PIN);
    MOTOR_RIGHT_IN2_PORT |= (1 << MOTOR_RIGHT_IN2_PIN);
    
    MOTOR_LEFT_IN3_PORT &= ~(1 << MOTOR_LEFT_IN3_PIN);
    MOTOR_LEFT_IN4_PORT |= (1 << MOTOR_LEFT_IN4_PIN);
    
    setRightMotorSpeed(speed);
    setLeftMotorSpeed(speed);
}

void turnRight(uint8_t speed) {
    MOTOR_RIGHT_IN1_PORT &= ~(1 << MOTOR_RIGHT_IN1_PIN);
    MOTOR_RIGHT_IN2_PORT |= (1 << MOTOR_RIGHT_IN2_PIN);
    
    MOTOR_LEFT_IN3_PORT |= (1 << MOTOR_LEFT_IN3_PIN);
    MOTOR_LEFT_IN4_PORT &= ~(1 << MOTOR_LEFT_IN4_PIN);
    
    setRightMotorSpeed(speed);
    setLeftMotorSpeed(speed);
}

void turnLeft(uint8_t speed) {
    MOTOR_RIGHT_IN1_PORT |= (1 << MOTOR_RIGHT_IN1_PIN);
    MOTOR_RIGHT_IN2_PORT &= ~(1 << MOTOR_RIGHT_IN2_PIN);
    
    MOTOR_LEFT_IN3_PORT &= ~(1 << MOTOR_LEFT_IN3_PIN);
    MOTOR_LEFT_IN4_PORT |= (1 << MOTOR_LEFT_IN4_PIN);
    
    setRightMotorSpeed(speed);
    setLeftMotorSpeed(speed);
}

void stopMotors(void) {
    MOTOR_RIGHT_IN1_PORT &= ~(1 << MOTOR_RIGHT_IN1_PIN);
    MOTOR_RIGHT_IN2_PORT &= ~(1 << MOTOR_RIGHT_IN2_PIN);
    MOTOR_LEFT_IN3_PORT &= ~(1 << MOTOR_LEFT_IN3_PIN);
    MOTOR_LEFT_IN4_PORT &= ~(1 << MOTOR_LEFT_IN4_PIN);
    
    setRightMotorSpeed(0);
    setLeftMotorSpeed(0);
}