#include "parking.h"
#include "pins.h"
#include "motors.h"
#include "sensors.h"

uint8_t detectParkingSlot(void) {
    static uint8_t spaceFound = 0;
    static uint16_t spaceLength = 0;
    static uint16_t previousRightDistance = 0;
    uint16_t currentRightDistance;
    
    currentRightDistance = measureRightDistance();
    
    if (!spaceFound && previousRightDistance < 10 && currentRightDistance > 20) {
        spaceFound = 1;
        spaceLength = 0;
    }
    
    if (spaceFound) {
        spaceLength++;
        
        if (currentRightDistance < 10) {
            uint16_t actualSpaceLength = spaceLength * VEHICLE_LENGTH / 100;
            spaceFound = 0;
            
            if (actualSpaceLength >= MIN_PARKING_SPACE + VEHICLE_LENGTH) {
                return 1;
            }
        }
    }
    
    previousRightDistance = currentRightDistance;
    return 0;
}

void performParallelParking(void) {
    moveForward(SPEED_SLOW);
    _delay_ms(1000);
    stopMotors();
    _delay_ms(500);
    
    turnRight(SPEED_NORMAL);
    _delay_ms(500);
    stopMotors();
    _delay_ms(300);
    moveBackward(SPEED_SLOW);
    _delay_ms(1200);
    
    turnLeft(SPEED_SLOW);
    _delay_ms(800);
    stopMotors();
    _delay_ms(300);
    
    moveForward(SPEED_SLOW);
    _delay_ms(400);
    stopMotors();
    
    LED_PARKING_PORT |= (1 << LED_PARKING_PIN);
}

void performPerpendicularParking(void) {
    moveForward(SPEED_SLOW);
    _delay_ms(500);
    stopMotors();
    _delay_ms(300);
    
    turnRight(SPEED_NORMAL);
    _delay_ms(700);
    stopMotors();
    _delay_ms(300);
    
    moveForward(SPEED_SLOW);
    _delay_ms(1000);
    stopMotors();
    
    LED_PARKING_PORT |= (1 << LED_PARKING_PIN);
}