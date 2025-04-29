#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"
#include "motors.h"
#include "bluetooth.h"
#include "parking.h"
#include "ultrasonic.h"


void setup() {
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

void loop() {
   
    if (Serial.available()) {
        char c = Serial.read();
        Serial.print("Received: ");
        Serial.println(c);
        process_bluetooth_commands(c);
    }
    static uint16_t left  = ultrasonic_read(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
    static uint16_t front = ultrasonic_read(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
    static uint16_t right = ultrasonic_read(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);

    if (is_parking_mode_active()) {
        parking_process(front, left, right);
    }
}