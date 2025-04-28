#ifndef PINS_H
#define PINS_H

#define SET_BIT(reg, bit)    ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))
#define READ_BIT(reg, bit)   ((reg) & (1 << (bit)))

// Ultrasonic sensor pins
#define LEFT_TRIG_PIN   PC0
#define LEFT_ECHO_PIN   PC1
#define FRONT_TRIG_PIN  PC2
#define FRONT_ECHO_PIN  PC3
#define RIGHT_TRIG_PIN  PC4
#define RIGHT_ECHO_PIN  PC5

// Motor control pins
// Right motors
#define RIGHT_IN1_PIN   PD2
#define RIGHT_IN2_PIN   PD3
#define RIGHT_EN_PIN    PD5  // PWM pin for speed control

// Left motors
#define LEFT_IN1_PIN    PD4
#define LEFT_IN2_PIN    PD7
#define LEFT_EN_PIN     PD6  // PWM pin for speed control

#define HEADLIGHT_PIN   PB0

// Speed definitions
#define SPEED_NORMAL    180  // 0-255 PWM value for normal speed
#define SPEED_SLOW      100  // 0-255 PWM value for slow speed
#define SPEED_VERY_SLOW 50   // 0-255 PWM value for very slow speed (for parking)

#endif // PINS_H