#include "motors.h"
#include "pins.h"

void motorInit(void)
{
    // Set all motor control pins as outputs
    MOTOR_FR_IN1_DDR |= (1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_DDR |= (1 << MOTOR_FR_IN2_PIN);
    MOTOR_FR_EN_DDR |= (1 << MOTOR_FR_EN_PIN);

    MOTOR_FL_IN1_DDR |= (1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_DDR |= (1 << MOTOR_FL_IN2_PIN);
    MOTOR_FL_EN_DDR |= (1 << MOTOR_FL_EN_PIN);

    MOTOR_RR_IN1_DDR |= (1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_DDR |= (1 << MOTOR_RR_IN2_PIN);
    MOTOR_RR_EN_DDR |= (1 << MOTOR_RR_EN_PIN);

    MOTOR_RL_IN1_DDR |= (1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_DDR |= (1 << MOTOR_RL_IN2_PIN);
    MOTOR_RL_EN_DDR |= (1 << MOTOR_RL_EN_PIN);

    // Initialize Timer1 for PWM (front motors)
    TCCR1A = (1 << WGM11) | (0 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    TCCR1A |= (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0);

    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10); // Prescaler 8

    ICR1 = 40000; // TOP value for 50Hz

    // Initialize Timer2 for PWM (rear right motor)
    TCCR2A = (1 << WGM21) | (1 << WGM20);             // Fast PWM mode
    TCCR2A |= (1 << COM2B1) | (0 << COM2B0);          // Non-inverting mode
    TCCR2B = (0 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler 32

    // Initialize Timer0 for PWM (rear left motor)
    TCCR0A = (1 << WGM01) | (1 << WGM00);             // Fast PWM mode
    TCCR0A |= (1 << COM0B1) | (0 << COM0B0);          // Non-inverting mode
    TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00); // Prescaler 64

    // Initialize PWM duty cycle to 0 (motors off)
    OCR1A = 0; // Front right
    OCR1B = 0; // Front left
    OCR2B = 0; // Rear right
    OCR0B = 0; // Rear left

    Serial.println("TCCR1A: " + String(TCCR1A, BIN)); // Should be 0b10100010
    Serial.println("TCCR1B: " + String(TCCR1B, BIN)); // Should be 0b00011010
}

void setMotorSpeed(uint8_t frontRight, uint8_t frontLeft, uint8_t rearRight, uint8_t rearLeft)
{
    OCR1A = (uint16_t)frontRight * (ICR1 / 255);
    OCR1B = (uint16_t)frontLeft * (ICR1 / 255);
    OCR2B = rearRight;
    OCR0B = rearLeft;
}

void moveForward(uint8_t speed)
{
    // Front right motor forward
    MOTOR_FR_IN1_PORT |= (1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT &= ~(1 << MOTOR_FR_IN2_PIN);

    // Front left motor forward
    MOTOR_FL_IN1_PORT |= (1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT &= ~(1 << MOTOR_FL_IN2_PIN);

    // Rear right motor forward
    MOTOR_RR_IN1_PORT |= (1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT &= ~(1 << MOTOR_RR_IN2_PIN);

    // Rear left motor forward
    MOTOR_RL_IN1_PORT |= (1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT &= ~(1 << MOTOR_RL_IN2_PIN);

    // Set all motors to same speed
    setMotorSpeed(speed, speed, speed, speed);
}

void moveBackward(uint8_t speed)
{
    // Front right motor backward
    MOTOR_FR_IN1_PORT &= ~(1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT |= (1 << MOTOR_FR_IN2_PIN);

    // Front left motor backward
    MOTOR_FL_IN1_PORT &= ~(1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT |= (1 << MOTOR_FL_IN2_PIN);

    // Rear right motor backward
    MOTOR_RR_IN1_PORT &= ~(1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT |= (1 << MOTOR_RR_IN2_PIN);

    // Rear left motor backward
    MOTOR_RL_IN1_PORT &= ~(1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT |= (1 << MOTOR_RL_IN2_PIN);

    // Set all motors to same speed
    setMotorSpeed(speed, speed, speed, speed);
}

void turnRight(uint8_t speed)
{
    // Front right motor backward
    MOTOR_FR_IN1_PORT &= ~(1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT |= (1 << MOTOR_FR_IN2_PIN);

    // Front left motor forward
    MOTOR_FL_IN1_PORT |= (1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT &= ~(1 << MOTOR_FL_IN2_PIN);

    // Rear right motor backward
    MOTOR_RR_IN1_PORT &= ~(1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT |= (1 << MOTOR_RR_IN2_PIN);

    // Rear left motor forward
    MOTOR_RL_IN1_PORT |= (1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT &= ~(1 << MOTOR_RL_IN2_PIN);

    // Set all motors to same speed
    setMotorSpeed(speed, speed, speed, speed);
}

void turnLeft(uint8_t speed)
{
    // Front right motor forward
    MOTOR_FR_IN1_PORT |= (1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT &= ~(1 << MOTOR_FR_IN2_PIN);

    // Front left motor backward
    MOTOR_FL_IN1_PORT &= ~(1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT |= (1 << MOTOR_FL_IN2_PIN);

    // Rear right motor forward
    MOTOR_RR_IN1_PORT |= (1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT &= ~(1 << MOTOR_RR_IN2_PIN);

    // Rear left motor backward
    MOTOR_RL_IN1_PORT &= ~(1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT |= (1 << MOTOR_RL_IN2_PIN);

    // Set all motors to same speed
    setMotorSpeed(speed, speed, speed, speed);
}

void turnRightForward(uint8_t speed)
{
    // Front right motor stopped
    MOTOR_FR_IN1_PORT &= ~(1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT &= ~(1 << MOTOR_FR_IN2_PIN);

    // Front left motor forward
    MOTOR_FL_IN1_PORT |= (1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT &= ~(1 << MOTOR_FL_IN2_PIN);

    // Rear right motor stopped
    MOTOR_RR_IN1_PORT &= ~(1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT &= ~(1 << MOTOR_RR_IN2_PIN);

    // Rear left motor forward
    MOTOR_RL_IN1_PORT |= (1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT &= ~(1 << MOTOR_RL_IN2_PIN);

    // Set only left motors to speed
    setMotorSpeed(0, speed, 0, speed);
}

void turnLeftForward(uint8_t speed)
{
    // Front right motor forward
    MOTOR_FR_IN1_PORT |= (1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT &= ~(1 << MOTOR_FR_IN2_PIN);

    // Front left motor stopped
    MOTOR_FL_IN1_PORT &= ~(1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT &= ~(1 << MOTOR_FL_IN2_PIN);

    // Rear right motor forward
    MOTOR_RR_IN1_PORT |= (1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT &= ~(1 << MOTOR_RR_IN2_PIN);

    // Rear left motor stopped
    MOTOR_RL_IN1_PORT &= ~(1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT &= ~(1 << MOTOR_RL_IN2_PIN);

    // Set only right motors to speed
    setMotorSpeed(speed, 0, speed, 0);
}

void turnRightBackward(uint8_t speed)
{
    // Front right motor stopped
    MOTOR_FR_IN1_PORT &= ~(1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT &= ~(1 << MOTOR_FR_IN2_PIN);

    // Front left motor backward
    MOTOR_FL_IN1_PORT &= ~(1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT |= (1 << MOTOR_FL_IN2_PIN);

    // Rear right motor stopped
    MOTOR_RR_IN1_PORT &= ~(1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT &= ~(1 << MOTOR_RR_IN2_PIN);

    // Rear left motor backward
    MOTOR_RL_IN1_PORT &= ~(1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT |= (1 << MOTOR_RL_IN2_PIN);

    // Set only left motors to speed
    setMotorSpeed(0, speed, 0, speed);
}

void turnLeftBackward(uint8_t speed)
{
    // Front right motor backward
    MOTOR_FR_IN1_PORT &= ~(1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT |= (1 << MOTOR_FR_IN2_PIN);

    // Front left motor stopped
    MOTOR_FL_IN1_PORT &= ~(1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT &= ~(1 << MOTOR_FL_IN2_PIN);

    // Rear right motor backward
    MOTOR_RR_IN1_PORT &= ~(1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT |= (1 << MOTOR_RR_IN2_PIN);

    // Rear left motor stopped
    MOTOR_RL_IN1_PORT &= ~(1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT &= ~(1 << MOTOR_RL_IN2_PIN);

    // Set only right motors to speed
    setMotorSpeed(speed, 0, speed, 0);
}

void rotateClockwise(uint8_t speed)
{
    // Front right motor backward
    MOTOR_FR_IN1_PORT &= ~(1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT |= (1 << MOTOR_FR_IN2_PIN);

    // Front left motor forward
    MOTOR_FL_IN1_PORT |= (1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT &= ~(1 << MOTOR_FL_IN2_PIN);

    // Rear right motor backward
    MOTOR_RR_IN1_PORT &= ~(1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT |= (1 << MOTOR_RR_IN2_PIN);

    // Rear left motor forward
    MOTOR_RL_IN1_PORT |= (1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT &= ~(1 << MOTOR_RL_IN2_PIN);

    // Set all motors to same speed
    setMotorSpeed(speed, speed, speed, speed);
}

void rotateCounterClockwise(uint8_t speed)
{
    // Front right motor forward
    MOTOR_FR_IN1_PORT |= (1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT &= ~(1 << MOTOR_FR_IN2_PIN);

    // Front left motor backward
    MOTOR_FL_IN1_PORT &= ~(1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT |= (1 << MOTOR_FL_IN2_PIN);

    // Rear right motor forward
    MOTOR_RR_IN1_PORT |= (1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT &= ~(1 << MOTOR_RR_IN2_PIN);

    // Rear left motor backward
    MOTOR_RL_IN1_PORT &= ~(1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT |= (1 << MOTOR_RL_IN2_PIN);

    // Set all motors to same speed
    setMotorSpeed(speed, speed, speed, speed);
}

void stopMotors(void)
{
    // All motors stopped
    MOTOR_FR_IN1_PORT &= ~(1 << MOTOR_FR_IN1_PIN);
    MOTOR_FR_IN2_PORT &= ~(1 << MOTOR_FR_IN2_PIN);

    MOTOR_FL_IN1_PORT &= ~(1 << MOTOR_FL_IN1_PIN);
    MOTOR_FL_IN2_PORT &= ~(1 << MOTOR_FL_IN2_PIN);

    MOTOR_RR_IN1_PORT &= ~(1 << MOTOR_RR_IN1_PIN);
    MOTOR_RR_IN2_PORT &= ~(1 << MOTOR_RR_IN2_PIN);

    MOTOR_RL_IN1_PORT &= ~(1 << MOTOR_RL_IN1_PIN);
    MOTOR_RL_IN2_PORT &= ~(1 << MOTOR_RL_IN2_PIN);

    // Set all motor speeds to 0
    setMotorSpeed(0, 0, 0, 0);
}