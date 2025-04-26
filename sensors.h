#ifndef SENSORS_H
#define SENSORS_H

#include <avr/io.h>
#include <util/delay.h>

void sensorInit(void);
uint16_t measureDistance(volatile uint8_t* triggerPort, uint8_t triggerPin, 
                         volatile uint8_t* echoPort, uint8_t echoPin);
uint16_t measureFrontDistance(void);
uint16_t measureRightDistance(void);
uint16_t measureLeftDistance(void);

#endif