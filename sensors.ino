#include "sensors.h"
#include "pins.h"

void sensorInit(void) {
    US_FRONT_TRIG_DDR |= (1 << US_FRONT_TRIG_PIN);
    US_RIGHT_TRIG_DDR |= (1 << US_RIGHT_TRIG_PIN);
    US_LEFT_TRIG_DDR |= (1 << US_LEFT_TRIG_PIN);
    
    US_FRONT_ECHO_DDR &= ~(1 << US_FRONT_ECHO_PIN);
    US_RIGHT_ECHO_DDR &= ~(1 << US_RIGHT_ECHO_PIN);
    US_LEFT_ECHO_DDR &= ~(1 << US_LEFT_ECHO_PIN);
    
    US_FRONT_ECHO_PORT |= (1 << US_FRONT_ECHO_PIN);
    US_RIGHT_ECHO_PORT |= (1 << US_RIGHT_ECHO_PIN);
    US_LEFT_ECHO_PORT |= (1 << US_LEFT_ECHO_PIN);
}

uint16_t measureDistance(volatile uint8_t* triggerPort, uint8_t triggerPin, 
                         volatile uint8_t* echoPort, uint8_t echoPin) {
    uint16_t duration = 0;
    
    *triggerPort &= ~(1 << triggerPin);
    _delay_us(2);
    
    *triggerPort |= (1 << triggerPin);
    _delay_us(10);
    *triggerPort &= ~(1 << triggerPin);
    
    while (!(*echoPort & (1 << echoPin))) {
        if (duration++ > 30000) return 0;
    }
    
    duration = 0;
    
    while (*echoPort & (1 << echoPin)) {
        _delay_us(1);
        duration++;
        if (duration > 23500) return 400;
    }
    
    return duration / 58;
}

uint16_t measureFrontDistance(void) {
    return measureDistance(&US_FRONT_TRIG_PORT, US_FRONT_TRIG_PIN, 
                           &US_FRONT_ECHO_PORT, US_FRONT_ECHO_PIN);
}

uint16_t measureRightDistance(void) {
    return measureDistance(&US_RIGHT_TRIG_PORT, US_RIGHT_TRIG_PIN, 
                           &US_RIGHT_ECHO_PORT, US_RIGHT_ECHO_PIN);
}

uint16_t measureLeftDistance(void) {
    return measureDistance(&US_LEFT_TRIG_PORT, US_LEFT_TRIG_PIN, 
                           &US_LEFT_ECHO_PORT, US_LEFT_ECHO_PIN);
}