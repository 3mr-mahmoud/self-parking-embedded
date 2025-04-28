// parking.c - Implementation of automatic parking functions
#include "parking.h"
#include "motors.h"
#include "pins.h"
#include "bluetooth.h"
#include "ultrasonic.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>

// Parking state machine
static ParkingState current_state = IDLE;

// Counters and flags for parking logic
static uint8_t parking_step = 0;
static uint16_t space_length = 0;
static uint8_t consecutive_open_readings = 0;
static bool parallel_space_detected = false;
static bool perpendicular_space_detected = false;

// Initialize parking system
void parking_init(void) {
    current_state = SCAN_FOR_SPACE;
    parking_step = 0;
    space_length = 0;
    consecutive_open_readings = 0;
    parallel_space_detected = false;
    perpendicular_space_detected = false;
}

// Detect if there's a suitable parking space
bool detect_parking_space(void) {
    uint16_t left = ultrasonic_read(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
    uint16_t front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
    uint16_t right = ultrasonic_read(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
    
    // First check if there's an obstacle in front
    if (front < 30) {
        // Too close to object in front, can't park here
        return false;
    }
    
    // Check for parallel parking spaces on the right
    if (right > 50) {
        consecutive_open_readings++;
        space_length += 2; // Approximate distance traveled between readings
        
        // If enough consecutive open space detected
        if (space_length > 60) { // Adjust this threshold based on vehicle size
            parallel_space_detected = true;
            return true;
        }
    } else {
        // Reset counters if we detect an obstacle
        consecutive_open_readings = 0;
        space_length = 0;
    }
    
    // Check for perpendicular parking space on the right
    if (right > 80 && front > 80) {
        perpendicular_space_detected = true;
        return true;
    }
    
    return false;
}

// Parallel parking procedure (right side)
void parallel_park_right(void) {
    switch(parking_step) {
        case 0:
            // Move forward a bit more to align with space
            move_forward(SPEED_SLOW);
            _delay_ms(1000);
            stop_motors();
            _delay_ms(500);
            parking_step = 1;
            break;
            
        case 1:
            // Turn right and move backward into the space
            rotate_right(SPEED_SLOW);
            _delay_ms(700);  // Adjust time based on desired angle
            stop_motors();
            _delay_ms(500);
            parking_step = 2;
            break;
            
        case 2:
            // Move backward
            move_backward(SPEED_SLOW);
            _delay_ms(1500);  // Adjust time based on space size
            stop_motors();
            _delay_ms(500);
            parking_step = 3;
            break;
            
        case 3:
            // Straighten out by turning wheels left while moving backward
            rotate_left(SPEED_SLOW);
            _delay_ms(500);  // Adjust time based on desired straightening
            stop_motors();
            _delay_ms(500);
            parking_step = 4;
            break;
            
        case 4:
            // Adjust position if needed
            move_forward(SPEED_VERY_SLOW);
            _delay_ms(500);  // Small adjustment forward
            stop_motors();
            parking_step = 5;
            break;
            
        case 5:
            // Parking complete
            current_state = PARKING_COMPLETE;
            parking_step = 0;
            break;
    }
}

// Parallel parking procedure (left side)
void parallel_park_left(void) {
    switch(parking_step) {
        case 0:
            // Move forward a bit more to align with space
            move_forward(SPEED_SLOW);
            _delay_ms(1000);
            stop_motors();
            _delay_ms(500);
            parking_step = 1;
            break;
            
        case 1:
            // Turn left and move backward into the space
            rotate_left(SPEED_SLOW);
            _delay_ms(700);  // Adjust time based on desired angle
            stop_motors();
            _delay_ms(500);
            parking_step = 2;
            break;
            
        case 2:
            // Move backward
            move_backward(SPEED_SLOW);
            _delay_ms(1500);  // Adjust time based on space size
            stop_motors();
            _delay_ms(500);
            parking_step = 3;
            break;
            
        case 3:
            // Straighten out by turning wheels right while moving backward
            rotate_right(SPEED_SLOW);
            _delay_ms(500);  // Adjust time based on desired straightening
            stop_motors();
            _delay_ms(500);
            parking_step = 4;
            break;
            
        case 4:
            // Adjust position if needed
            move_forward(SPEED_VERY_SLOW);
            _delay_ms(500);  // Small adjustment forward
            stop_motors();
            parking_step = 5;
            break;
            
        case 5:
            // Parking complete
            current_state = PARKING_COMPLETE;
            parking_step = 0;
            break;
    }
}

// Perpendicular parking procedure
void perpendicular_park(void) {
    switch(parking_step) {
        case 0:
            // Move forward a bit more to align with space
            move_forward(SPEED_SLOW);
            _delay_ms(500);
            stop_motors();
            _delay_ms(500);
            parking_step = 1;
            break;
            
        case 1:
            // Turn 90 degrees right
            rotate_right(SPEED_SLOW);
            _delay_ms(1500);  // Adjust time based on desired angle (90 degrees)
            stop_motors();
            _delay_ms(500);
            parking_step = 2;
            break;
            
        case 2:
            // Move forward into the space
            move_forward(SPEED_SLOW);
            _delay_ms(1500);  // Adjust time based on space size
            stop_motors();
            _delay_ms(500);
            parking_step = 3;
            break;
            
        case 3:
            // Adjust position if needed
            move_backward(SPEED_VERY_SLOW);
            _delay_ms(300);  // Small adjustment backward
            stop_motors();
            parking_step = 4;
            break;
            
        case 4:
            // Parking complete
            current_state = PARKING_COMPLETE;
            parking_step = 0;
            break;
    }
}

// Main parking process function - to be called repeatedly in main loop
void parking_process(void) {
    // If parking mode is not active, do nothing
    if (!is_parking_mode_active()) {
        current_state = IDLE;
        return;
    }
    
    // Handle parking state machine
    switch(current_state) {
        case SCAN_FOR_SPACE:
            // Move forward slowly while scanning
            move_forward(SPEED_VERY_SLOW);
            
            // Check for suitable parking space
            if (detect_parking_space()) {
                stop_motors();
                _delay_ms(500);
                
                // Decide which parking maneuver to use
                if (perpendicular_space_detected) {
                    perpendicular_space_detected = false;
                    current_state = PERPENDICULAR_PARKING;
                } else if (parallel_space_detected) {
                    parallel_space_detected = false;
                    current_state = PARALLEL_PARKING_RIGHT; // Default to right side
                }
                
                // Reset step counter for the parking procedure
                parking_step = 0;
            }
            break;
            
        case PARALLEL_PARKING_RIGHT:
            parallel_park_right();
            break;
            
        case PARALLEL_PARKING_LEFT:
            parallel_park_left();
            break;
            
        case PERPENDICULAR_PARKING:
            perpendicular_park();
            break;
            
        case PARKING_COMPLETE:
            // Flash headlight to indicate parking is complete
            SET_BIT(PORTB, HEADLIGHT_PIN);
            _delay_ms(300);
            CLEAR_BIT(PORTB, HEADLIGHT_PIN);
            _delay_ms(300);
            SET_BIT(PORTB, HEADLIGHT_PIN);
            _delay_ms(300);
            CLEAR_BIT(PORTB, HEADLIGHT_PIN);
            
            // Reset parking mode
            set_parking_mode(false);
            current_state = IDLE;
            break;
            
        case IDLE:
            // Do nothing in idle state
            break;
    }
}