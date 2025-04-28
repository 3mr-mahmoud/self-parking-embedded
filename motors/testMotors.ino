#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"
#include "motors.h"

void print_status(const char* action) {
    Serial.println(action);
}

int main(void) {
    // Initialize serial communication
    Serial.begin(9600);
    Serial.println("Motor Test Program Starting");

    ultrasonic_init();

    // Initialize motors
    motors_init();
    _delay_ms(1000);  // Startup delay
    
    
    while (1) {


        uint16_t left  = ultrasonic_read(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
        uint16_t front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
        uint16_t right = ultrasonic_read(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);

        // Use these distances (in cm) as needed...
        Serial.println("Left: " + String(left) + 
                   " Front: " + String(front) + 
                   " Right: " + String(right));
        
        _delay_ms(1000);

        // Test each movement with both normal and slow speeds
        
        // Forward movement test - Normal speed
        print_status("Moving forward (NORMAL speed)");
        move_forward(SPEED_NORMAL);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Forward movement test - Slow speed
        print_status("Moving forward (SLOW speed)");
        move_forward(SPEED_SLOW);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Backward movement test - Normal speed
        print_status("Moving backward (NORMAL speed)");
        move_backward(SPEED_NORMAL);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Backward movement test - Slow speed
        print_status("Moving backward (SLOW speed)");
        move_backward(SPEED_SLOW);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Turn right test - Normal speed
        print_status("Turning right (NORMAL speed)");
        turn_right(SPEED_NORMAL);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Turn right test - Slow speed
        print_status("Turning right (SLOW speed)");
        turn_right(SPEED_SLOW);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Turn left test - Normal speed
        print_status("Turning left (NORMAL speed)");
        turn_left(SPEED_NORMAL);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Turn left test - Slow speed
        print_status("Turning left (SLOW speed)");
        turn_left(SPEED_SLOW);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Rotate right test - Normal speed
        print_status("Rotating right (NORMAL speed)");
        rotate_right(SPEED_NORMAL);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Rotate right test - Slow speed
        print_status("Rotating right (SLOW speed)");
        rotate_right(SPEED_SLOW);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Rotate left test - Normal speed
        print_status("Rotating left (NORMAL speed)");
        rotate_left(SPEED_NORMAL);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // Rotate left test - Slow speed
        print_status("Rotating left (SLOW speed)");
        rotate_left(SPEED_SLOW);
        _delay_ms(3000);
        stop_motors();
        _delay_ms(1000);
        
        // All tests completed, wait before repeating
        print_status("All movement tests completed. Waiting 5 seconds before repeating...");
        _delay_ms(5000);
    }
    
    return 0;  // Should never reach here
}