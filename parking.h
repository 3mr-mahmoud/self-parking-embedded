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
void parking_process(uint16_t front, uint16_t left, uint16_t right);
bool detect_parking_space(uint16_t front, uint16_t left, uint16_t right);
void parallel_park_right(void);
void parallel_park_left(void);
void perpendicular_park(void);

#endif // PARKING_H