#include "parking.h"
#include "pins.h"
#include "motors.h"
#include "sensors.h"

uint8_t detectParkingSlot(void)
{
    static uint8_t spaceFound = 0;
    static uint16_t spaceLength = 0;
    static uint16_t previousRightDistance = 0;
    uint16_t currentRightDistance;

    currentRightDistance = measureRightDistance();

    if (!spaceFound && previousRightDistance < 10 && currentRightDistance > 20)
    {
        spaceFound = 1;
        spaceLength = 0;
    }

    if (spaceFound)
    {
        spaceLength++;

        if (currentRightDistance < 10)
        {
            uint16_t actualSpaceLength = spaceLength * VEHICLE_LENGTH / 100;
            spaceFound = 0;

            if (actualSpaceLength >= MIN_PARKING_SPACE + VEHICLE_LENGTH)
            {
                return 1;
            }
        }
    }

    previousRightDistance = currentRightDistance;
    return 0;
}

void performParallelParking(void)
{
    // Step 1: Move forward slightly to align with parking space
    moveForward(SPEED_SLOW);
    _delay_ms(1000);
    stopMotors();
    _delay_ms(500);

    // Step 2: Turn right sharply while moving backward to angle into space
    turnRightBackward(SPEED_NORMAL);
    _delay_ms(800);
    stopMotors();
    _delay_ms(300);

    // Step 3: Back up straight until mostly in the spot
    moveBackward(SPEED_SLOW);
    _delay_ms(1000);
    stopMotors();
    _delay_ms(300);

    // Step 4: Turn left while backing up to align with curb
    turnLeftBackward(SPEED_SLOW);
    _delay_ms(700);
    stopMotors();
    _delay_ms(300);

    // Step 5: Small forward adjustment to center in spot
    moveForward(SPEED_SLOW);
    _delay_ms(300);
    stopMotors();

    // Signal parking complete
    LED_PARKING_PORT |= (1 << LED_PARKING_PIN);
}

void performPerpendicularParking(void)
{
    // Step 1: Position the car in front of the parking space
    moveForward(SPEED_SLOW);
    _delay_ms(500);
    stopMotors();
    _delay_ms(300);

    // Step 2: Rotate clockwise 45 degrees
    rotateClockwise(SPEED_NORMAL);
    _delay_ms(500);
    stopMotors();
    _delay_ms(300);

    // Step 3: Move forward while turning right to align
    turnRightForward(SPEED_SLOW);
    _delay_ms(700);
    stopMotors();
    _delay_ms(300);

    // Step 4: Move forward into the parking space
    moveForward(SPEED_SLOW);
    _delay_ms(800);
    stopMotors();

    // Signal parking complete
    LED_PARKING_PORT |= (1 << LED_PARKING_PIN);
}