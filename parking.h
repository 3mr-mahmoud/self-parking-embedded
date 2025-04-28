// parking.h - Functions for automatic parking features

#ifndef PARKING_H
#define PARKING_H

#include <stdbool.h>

// Parking states
typedef enum {
    SCAN_FOR_SPACE,
    PARALLEL_PARKING_RIGHT,
    PARALLEL_PARKING_LEFT,
    PERPENDICULAR_PARKING,
    PARKING_COMPLETE,
    IDLE
} ParkingState;

// Function prototypes
void parking_init(void);
void parking_process(void);
bool detect_parking_space(void);
void parallel_park_right(void);
void parallel_park_left(void);
void perpendicular_park(void);

#endif // PARKING_H