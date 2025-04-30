#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"
#include "motors.h"
#include "bluetooth.h"
#include "parking.h"
#include "ultrasonic.h"

static uint16_t left;
static uint16_t front;
static uint16_t right;

void move_forward(uint8_t speed)
{
    if (front < 5)
    {
        stop_motors();
        return;
    }
    // Right motors direction
    CLEAR_BIT(PORTD, RIGHT_IN1_PIN);
    SET_BIT(PORTB, RIGHT_IN2_PIN);

    // Left motors direction
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    SET_BIT(PORTD, LEFT_IN2_PIN);

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
}

void move_forward_detect(uint8_t speed)
{
    while (front > 5)
    {
        // Right motors direction
        CLEAR_BIT(PORTD, RIGHT_IN1_PIN);
        SET_BIT(PORTB, RIGHT_IN2_PIN);

        // Left motors direction
        CLEAR_BIT(PORTD, LEFT_IN1_PIN);
        SET_BIT(PORTD, LEFT_IN2_PIN);

        // Set speed
        OCR0A = speed; // Right motors speed
        OCR0B = speed; // Left motors speed
        _delay_ms(50);
        stop_motors();
        _delay_ms(500);
        front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
    }
    stop_motors();
    return;
}

void move_backward(uint8_t speed)
{
    // Right motors direction
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTB, RIGHT_IN2_PIN);

    // Left motors direction
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
}

void turn_right(uint8_t speed)
{
    if (right < 5)
    {
        stop_motors();
        return;
    }
    // Right motors faster
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTB, RIGHT_IN2_PIN);

    // Left motors slower
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Set different speeds for turning
    OCR0A = speed / 2; // Right motors slower
    OCR0B = speed;     // Left motors normal
}

void turn_left(uint8_t speed)
{
    if (left < 5)
    {
        stop_motors();
        return;
    }
    // Right motors slower
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTB, RIGHT_IN2_PIN);

    // Left motors faster
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Set different speeds for turning
    OCR0A = speed;     // Right motors normal
    OCR0B = speed / 2; // Left motors slower
}

void rotate_right(uint8_t speed)
{
    // Right motors forward
    SET_BIT(PORTD, RIGHT_IN1_PIN);
    CLEAR_BIT(PORTB, RIGHT_IN2_PIN);

    // Left motors backward
    CLEAR_BIT(PORTD, LEFT_IN1_PIN);
    SET_BIT(PORTD, LEFT_IN2_PIN);

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
}

// void rotate_right_detect(uint8_t speed)
// {
//   static bool state1_satisified = false;
//   static bool state2_satisified = false;
  
//     while (!state1_satisified || !state2_satisified)
//     {
        

//         // Right motors forward
//         SET_BIT(PORTD, RIGHT_IN1_PIN);
//         CLEAR_BIT(PORTB, RIGHT_IN2_PIN);

//         // Left motors backward
//         CLEAR_BIT(PORTD, LEFT_IN1_PIN);
//         SET_BIT(PORTD, LEFT_IN2_PIN);

//         // Set speed
//         OCR0A = speed; // Right motors speed
//         OCR0B = speed; // Left motors speed
//         _delay_ms(50);
//         stop_motors();
//         _delay_ms(500);
//         front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
//         Serial.print("Front : ");
//         Serial.println(front);
//         if(front <= 15) {
//           state1_satisified = true;
//           Serial.println("state1 satisified")
//           continue;
//         }

//         if(state1_satisified && front > 25) {
//           Serial.println("state2 satisified")
//           state2_satisified = true;
//         }

      

//     }
//     stop_motors();
//     _delay_ms(200);
//     return;
// }

void rotate_right_detect(uint8_t speed)
{
       for(int i=0;i<32;i++){
        
        // Right motors forward
        SET_BIT(PORTD, RIGHT_IN1_PIN);
        CLEAR_BIT(PORTB, RIGHT_IN2_PIN);

        // Left motors backward
        CLEAR_BIT(PORTD, LEFT_IN1_PIN);
        SET_BIT(PORTD, LEFT_IN2_PIN);

        // Set speed
        OCR0A = speed; // Right motors speed
        OCR0B = speed; // Left motors speed
        _delay_ms(50);
        stop_motors();
        _delay_ms(500);
            
        left = ultrasonic_read(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
        right = ultrasonic_read(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
        front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);

        if(left > 30 && right > 50 && front > 25) {
          break;
        }
       }
    return;
}

void rotate_left(uint8_t speed)
{
    // Right motors backward
    CLEAR_BIT(PORTD, RIGHT_IN1_PIN);
    SET_BIT(PORTB, RIGHT_IN2_PIN);

    // Left motors forward
    SET_BIT(PORTD, LEFT_IN1_PIN);
    CLEAR_BIT(PORTD, LEFT_IN2_PIN);

    // Set speed
    OCR0A = speed; // Right motors speed
    OCR0B = speed; // Left motors speed
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
        parking_process(front, left, right);
    }
}