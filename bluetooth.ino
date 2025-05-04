// bluetooth.c - Implementation of Bluetooth control functions
#include "bluetooth.h"
#include "motors.h"
#include "pins.h"
#include <util/delay.h>

#define SET_BIT(reg, bit)    ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))



// Global variables
static bool parking_mode = false;
static uint8_t current_speed = SPEED_NORMAL;
static MovementState current_movement = STATE_STOPPED;

// Initialize Bluetooth module
void bluetooth_init(void) {
    // HC-05 communicates via UART, which should already be initialized
    // Setup additional pins
    SET_BIT(DDRB, HEADLIGHT_PIN);  // Set headlight pin as output
    
    // Ensure headlight and buzzer are off initially
    CLEAR_BIT(PORTB, HEADLIGHT_PIN);
}


bool is_parking_mode_active(void) {
    return parking_mode;
}

void apply_current_movement() {
    switch(current_movement) {
        case STATE_FORWARD:
            move_forward(current_speed);
            break;
        case STATE_BACKWARD:
            move_backward(current_speed);
            break;
        case STATE_RIGHT:
            rotate_right(current_speed);
            break;
        case STATE_LEFT:
            rotate_left(current_speed);
            break;
        case STATE_FORWARD_LEFT:
            turn_left(current_speed);
            break;
        case STATE_FORWARD_RIGHT:
            turn_right(current_speed);
            break;
        case STATE_BACKWARD_LEFT:
            OCR0B = current_speed / 4;
            OCR0A = current_speed;
            move_backward(current_speed);
            break;
        case STATE_BACKWARD_RIGHT:
            OCR0A = current_speed / 4;
            OCR0B = current_speed;
            move_backward(current_speed);
            break;
        case STATE_STOPPED:
            stop_motors();
            break;
    }
}

// Set the car's speed based on the command
void set_speed(char speed_level) {
    Serial.println("speed received");
    switch(speed_level) {
        case '0': current_speed = 0; break;
        case '1': current_speed = 25; break;
        case '2': current_speed = 50; break;
        case '3': current_speed = 75; break;
        case '4': current_speed = 100; break;
        case '5': current_speed = 125; break;
        case '6': current_speed = 150; break;
        case '7': current_speed = 175; break;
        case '8': current_speed = 200; break;
        case '9': current_speed = 255; break;
        default: break;
    }

    Serial.println(current_speed);
    
    // Reapply current movement with new speed if we're moving
    if (current_movement != STATE_STOPPED) {
        apply_current_movement();
    }
}

// Toggle parking mode
void set_parking_mode(bool enable) {
    parking_mode = enable;
    
    if (enable) {
        current_state = SCAN_FOR_SPACE;
        // Headlight flash to indicate parking mode ON
        SET_BIT(PORTB, HEADLIGHT_PIN);
        _delay_ms(200);
        CLEAR_BIT(PORTB, HEADLIGHT_PIN);
        _delay_ms(200);
        SET_BIT(PORTB, HEADLIGHT_PIN);
        current_movement = STATE_STOPPED;
        stop_motors();
    } else {
        CLEAR_BIT(PORTB, HEADLIGHT_PIN);
    }
}

// Process incoming Bluetooth commands
void process_bluetooth_commands(char cmd) {
    // If we're in parking mode, only the headlight cmd can exit
    if (parking_mode) {
        if (cmd == CMD_HEADLIGHT_ON || cmd == CMD_HEADLIGHT_OFF) {
            set_parking_mode(false);
        }
        return;
    }
        
    // Process commands
    switch(cmd) {
        case CMD_FORWARD:
            current_movement = STATE_FORWARD;
            move_forward(current_speed);
            break;
            
        case CMD_BACKWARD:
            current_movement = STATE_BACKWARD;
            move_backward(current_speed);
            break;
            
        case CMD_RIGHT:
            current_movement = STATE_RIGHT;
            rotate_right(current_speed);
            break;
            
        case CMD_LEFT:
            current_movement = STATE_LEFT;
            rotate_left(current_speed);
            break;
            
        case CMD_FORWARD_LEFT:
            current_movement = STATE_FORWARD_LEFT;
            turn_left(current_speed);
            break;
            
        case CMD_FORWARD_RIGHT:
            current_movement = STATE_FORWARD_RIGHT;
            turn_right(current_speed);
            break;
            
        case CMD_BACKWARD_LEFT:
            current_movement = STATE_BACKWARD_LEFT;
            OCR0B = current_speed / 4;
            OCR0A = current_speed;
            move_backward(current_speed);
            break;
            
        case CMD_BACKWARD_RIGHT:
            current_movement = STATE_BACKWARD_RIGHT;
            OCR0A = current_speed / 4;
            OCR0B = current_speed;
            move_backward(current_speed);
            break;
            
        case CMD_STOP:
            current_movement = STATE_STOPPED;
            stop_motors();
            break;
            
        case CMD_HEADLIGHT_ON:
            SET_BIT(PORTB, HEADLIGHT_PIN);
            set_parking_mode(true);
            break;
            
        case CMD_HEADLIGHT_OFF:
            CLEAR_BIT(PORTB, HEADLIGHT_PIN);
            break;

        
        case CMD_SPEED_0:
        case CMD_SPEED_1:
        case CMD_SPEED_2:
        case CMD_SPEED_3:
        case CMD_SPEED_4:
        case CMD_SPEED_5:
        case CMD_SPEED_6:
        case CMD_SPEED_7:
        case CMD_SPEED_8:
        case CMD_SPEED_9:
            set_speed(cmd);
            break;
            
        default:
            // Unrecognized command
            break;
    }
}