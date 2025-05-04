#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"
#include "motors.h"
#include "bluetooth.h"
#include "parking.h"
#include "ultrasonic.h"

uint16_t left;
uint16_t front;
uint16_t right;

// Parking state machine
ParkingState current_state = IDLE;

// Counters and flags for parking logic
static uint8_t parking_step = 0;
static uint16_t space_length = 0;
static uint8_t consecutive_open_readings = 0;
static bool parallel_space_detected = false;
static bool perpendicular_right_space_detected = false;
static bool perpendicular_left_space_detected = false;



void sampleReadings(DIR dir, int count = 5) {
    for(int i =0; i < count; i++) {
        if(dir == RIGHT) {
            right = ultrasonic_read(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
        } else if (dir == LEFT){
            left = ultrasonic_read(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
        } else {
            front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
        }
        _delay_ms(100);
    }
}

// Initialize parking system
void parking_init(void)
{
    current_state = SCAN_FOR_SPACE;
    parking_step = 0;
    space_length = 0;
    consecutive_open_readings = 0;
    parallel_space_detected = false;
    perpendicular_right_space_detected = false;
    perpendicular_left_space_detected = false;
}

// Perpendicular parking procedure
void perpendicular_park_right(void)
{

    rotate_right_detect(SPEED_VERY_SLOW);

    sampleReadings(FRONT);

    move_forward_detect(SPEED_VERY_SLOW);

    current_state = PARKING_COMPLETE;

    return;
}

void perpendicular_park_left(void)
{

    rotate_left_detect(SPEED_VERY_SLOW);

    sampleReadings(FRONT);

    move_forward_detect(SPEED_VERY_SLOW);

    current_state = PARKING_COMPLETE;

    return;
}

void move_forward(uint8_t speed)
{
     // Right motors direction
    SET_BIT(PORTB, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);

    // Left motors direction
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
}




void move_forward_detect(uint8_t speed)
{
    while (front > 5)
    {
        move_forward(speed);
        _delay_ms(40);
        stop_motors();
        sampleReadings(FRONT);
    }
    stop_motors();
    return;
}

void move_backward(uint8_t speed)
{
    // Right motors direction
    CLEAR_BIT(PORTB, RIGHT_IN1_PIN);
    SET_BIT(PORTD, RIGHT_IN2_PIN);

    // Left motors direction
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    SET_BIT(PORTD, LEFT_IN2_PIN);

   

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
}

void turn_right(uint8_t speed)
{
    // Right motors slower
    SET_BIT(PORTB, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);

    // Left motors faster
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Set different speeds for turning
    OCR0A = speed / 2; // Right motors slower
    OCR0B = speed;     // Left motors normal
}

void turn_left(uint8_t speed)
{

    // Right motors faster
    SET_BIT(PORTB, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);

    // Left motors slower
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);
    

    // Set different speeds for turning
    OCR0A = speed;     // Right motors normal
    OCR0B = speed / 2; // Left motors slower
}

void rotate_right(uint8_t speed)
{
    // Right motors backward
    CLEAR_BIT(PORTB, RIGHT_IN1_PIN);
    SET_BIT(PORTD, RIGHT_IN2_PIN);

    // Left motors forward
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
}

void rotate_right_detect(uint8_t speed)
{
       for(int i=0;i<4;i++){
        rotate_right(speed);
        _delay_ms(66);
        stop_motors();
        sampleReadings(LEFT,1);
        sampleReadings(RIGHT,1);
        sampleReadings(FRONT,1);
       }
    return;
}



void rotate_left(uint8_t speed)
{
    // Right motors forward
    SET_BIT(PORTB, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTD, RIGHT_IN2_PIN);

    // Left motors backward
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    SET_BIT(PORTD, LEFT_IN2_PIN);

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
}


// 4 iterations with delay 60 ms

void rotate_left_detect(uint8_t speed)
{
       for(int i=0;i<4;i++){
        rotate_left(speed);
        _delay_ms(60);
        stop_motors();
        sampleReadings(LEFT,1);
        sampleReadings(RIGHT,1);
        sampleReadings(FRONT,1);
       }
    return;
}



bool detect_parking_space()
{

    // First check if there's an obstacle in front
    if (front < 20)
    {
        return false;
    }

    Serial.println("right");
    Serial.println(right);

    Serial.println("left");
    Serial.println(left);

    Serial.println("front");
    Serial.println(front);


    if (right > 30 && right <= 50)
    {
        perpendicular_right_space_detected = true;
        return true;
    }

    
    if (left > 30 && left <= 50)
    {
        perpendicular_left_space_detected = true;
        return true;
    }

    return false;
}

void parking_process()
{
    // If parking mode is not active, do nothing
    if (!is_parking_mode_active())
    {
        current_state = IDLE;
        return;
    }

    // Handle parking state machine
    switch (current_state)
    {
    case SCAN_FOR_SPACE:
        // Move forward slowly while scanning
        move_forward(SPEED_VERY_SLOW);
        _delay_ms(40);
        stop_motors();
        sampleReadings(FRONT,2);
        sampleReadings(RIGHT,2);
        sampleReadings(LEFT,2);
        // Check for suitable parking space
        if (detect_parking_space())
        {
            stop_motors();
            _delay_ms(500);

            for(int i = 0; i < 3; i++){
              move_forward(SPEED_VERY_SLOW);
              _delay_ms(40);
              stop_motors();
              _delay_ms(500);
            }

            // Decide which parking maneuver to use
            if (perpendicular_right_space_detected)
            {
                perpendicular_right_space_detected = false;
                current_state = PERPENDICULAR_PARKING_RIGHT;
            }
            else if (perpendicular_left_space_detected)
            {
                perpendicular_left_space_detected = false;
                current_state = PERPENDICULAR_PARKING_LEFT;
            }

            // Reset step counter for the parking procedure
            parking_step = 0;
        }
        break;

    case PERPENDICULAR_PARKING_RIGHT:
        perpendicular_park_right();
        break;

    case PERPENDICULAR_PARKING_LEFT:
        perpendicular_park_left();
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

void setup()
{
    // Initialize serial communication for Bluetooth
    Serial.begin(9600);
    Serial.println("Ready to receive commands");

    // Initialize all components
    ultrasonic_init();
    timer1_init();
    motors_init();
    bluetooth_init();
    parking_init();
}


void loop()
{

    if (Serial.available())
    {
        char c = Serial.read();
        Serial.print("Received: ");
        Serial.println(c);
        process_bluetooth_commands(c);
    }
    left = ultrasonic_read(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
    front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
    right = ultrasonic_read(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);

    if (is_parking_mode_active())
    {
        parking_process();
    }
}