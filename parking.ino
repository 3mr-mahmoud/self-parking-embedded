// parking.c - Implementation of automatic parking functions
#include "parking.h"
#include "motors.h"
#include "pins.h"
#include "bluetooth.h"
#include "ultrasonic.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>



// Detect if there's a suitable parking space


// Parallel parking procedure (right side)
void parallel_park_right(void)
{
    switch (parking_step)
    {
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
        _delay_ms(700); // Adjust time based on desired angle
        stop_motors();
        _delay_ms(500);
        parking_step = 2;
        break;

    case 2:
        // Move backward
        move_backward(SPEED_SLOW);
        _delay_ms(1500); // Adjust time based on space size
        stop_motors();
        _delay_ms(500);
        parking_step = 3;
        break;

    case 3:
        // Straighten out by turning wheels left while moving backward
        rotate_left(SPEED_SLOW);
        _delay_ms(500); // Adjust time based on desired straightening
        stop_motors();
        _delay_ms(500);
        parking_step = 4;
        break;

    case 4:
        // Adjust position if needed
        move_forward(SPEED_VERY_SLOW);
        _delay_ms(500); // Small adjustment forward
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
void parallel_park_left(void)
{
    switch (parking_step)
    {
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
        _delay_ms(700); // Adjust time based on desired angle
        stop_motors();
        _delay_ms(500);
        parking_step = 2;
        break;

    case 2:
        // Move backward
        move_backward(SPEED_SLOW);
        _delay_ms(1500); // Adjust time based on space size
        stop_motors();
        _delay_ms(500);
        parking_step = 3;
        break;

    case 3:
        // Straighten out by turning wheels right while moving backward
        rotate_right(SPEED_SLOW);
        _delay_ms(500); // Adjust time based on desired straightening
        stop_motors();
        _delay_ms(500);
        parking_step = 4;
        break;

    case 4:
        // Adjust position if needed
        move_forward(SPEED_VERY_SLOW);
        _delay_ms(500); // Small adjustment forward
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

