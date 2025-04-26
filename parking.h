#ifndef PARKING_H
#define PARKING_H

#include <avr/io.h>
#include <util/delay.h>

uint8_t detectParkingSlot(void);
void performParallelParking(void);
void performPerpendicularParking(void);

#endif