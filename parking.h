// parking.h - Functions for automatic parking features

#ifndef PARKING_H
#define PARKING_H

#include <stdbool.h>

// Parking states
typedef enum {
    SCAN_FOR_SPACE,
    PERPENDICULAR_PARKING_LEFT,
    PERPENDICULAR_PARKING_RIGHT,
    PARKING_COMPLETE,
    IDLE
} ParkingState;

typedef enum {
    RIGHT,
    LEFT,
    FRONT
} DIR;

// Function prototypes
void parking_init(void);
void parking_process(void);
bool detect_parking_space(void);
void parallel_park_right(void);
void parallel_park_left(void);
void perpendicular_park(void);

#endif // PARKING_H