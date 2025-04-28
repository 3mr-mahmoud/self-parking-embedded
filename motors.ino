#include "motors.h"
#include "pins.h"


void motors_init() {
    // Configure motor control pins as outputs
    // Right motors
    SET_BIT(DDRD, RIGHT_IN1_PIN);
    SET_BIT(DDRD, RIGHT_IN2_PIN);
    SET_BIT(DDRD, RIGHT_EN_PIN);
    
    // Left motors
    SET_BIT(DDRD, LEFT_IN1_PIN);
    SET_BIT(DDRD, LEFT_IN2_PIN);
    SET_BIT(DDRD, LEFT_EN_PIN);
    
    // Initialize PWM for speed control
    // Fast PWM mode, non-inverting
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    
    // Initial state: motors stopped
    stop_motors();
}

void move_forward(uint8_t speed) {
    // Right motors direction
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);
    
    // Left motors direction
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);
    
    // Set speed
    OCR0A = speed;  // Right motors speed
    OCR0B = speed;  // Left motors speed
}


void move_backward(uint8_t speed) {
    // Right motors direction
    CLEAR_BIT(PORTD, RIGHT_IN1_PIN);
    SET_BIT(PORTD, RIGHT_IN2_PIN);
    
    // Left motors direction
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    SET_BIT(PORTD, LEFT_IN2_PIN);
    
    // Set speed
    OCR0A = speed;  // Right motors speed
    OCR0B = speed;  // Left motors speed
}

void turn_right(uint8_t speed) {
    // Right motors slower
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);
    
    // Left motors faster
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);
    
    // Set different speeds for turning
    OCR0A = speed / 2;  // Right motors slower
    OCR0B = speed;      // Left motors normal
}


void turn_left(uint8_t speed) {
    // Right motors faster
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);
    
    // Left motors slower
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);
    
    // Set different speeds for turning
    OCR0A = speed;      // Right motors normal
    OCR0B = speed / 2;  // Left motors slower
}


void rotate_right(uint8_t speed) {
    // Right motors backward
    CLEAR_BIT(PORTD, RIGHT_IN1_PIN);
    SET_BIT(PORTD, RIGHT_IN2_PIN);
    
    // Left motors forward
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);
    
    // Set speed
    OCR0A = speed;  // Right motors speed
    OCR0B = speed;  // Left motors speed
}


void rotate_left(uint8_t speed) {
    // Right motors forward
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);
    
    // Left motors backward
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    SET_BIT(PORTD, LEFT_IN2_PIN);
    
    // Set speed
    OCR0A = speed;  // Right motors speed
    OCR0B = speed;  // Left motors speed
}


void stop_motors() {
    // Stop all motors
    CLEAR_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);
    
    // Disable PWM
    OCR0A = 0;
    OCR0B = 0;
}

