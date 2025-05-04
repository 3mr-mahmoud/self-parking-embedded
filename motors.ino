#include "motors.h"
#include "pins.h"

void motors_init()
{
    // Configure motor control pins as outputs
    // Right motors
    SET_BIT(DDRB, RIGHT_IN1_PIN);
    SET_BIT(DDRD, RIGHT_IN2_PIN);
    SET_BIT(DDRD, RIGHT_EN_PIN);

    // Left motors
    SET_BIT(DDRD, LEFT_IN1_PIN);
    SET_BIT(DDRD, LEFT_IN2_PIN);
    SET_BIT(DDRD, LEFT_EN_PIN);

    // Initialize PWM for speed control
    // Fast PWM mode, non-inverting
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

    TCCR0B = (1 << CS00);
    // Initial state: motors stopped
    stop_motors();
}

void stop_motors()
{
    // Stop all motors
    CLEAR_BIT(PORTB, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Disable PWM
    OCR0A = 0;
    OCR0B = 0;
}
