#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pins.h"
#include "motors.h"
#include "sensors.h"
#include "uart.h"
#include "parking.h"

typedef enum {
    STATE_IDLE,
    STATE_SEARCHING,
    STATE_PARALLEL_PARKING,
    STATE_PERPENDICULAR_PARKING,
    STATE_PARKED
} SystemState;

volatile SystemState currentState = STATE_IDLE;
volatile uint16_t frontDistance = 0;
volatile uint16_t rightDistance = 0;
volatile uint16_t leftDistance = 0;
volatile uint8_t parkingType = 0;

void init(void) {
    motorInit();
    sensorInit();
    uartInit();
    
    LED_STATUS_DDR |= (1 << LED_STATUS_PIN);
    LED_PARKING_DDR |= (1 << LED_PARKING_PIN);
    LED_STATUS_PORT |= (1 << LED_STATUS_PIN);
    
    sei();
}

ISR(USART_RX_vect) {
    uint8_t receivedByte = UDR0;
    
    switch (receivedByte) {
        case 'S':
            currentState = STATE_SEARCHING;
            LED_STATUS_PORT ^= (1 << LED_STATUS_PIN);
            break;
            
        case 'P':
            currentState = STATE_PARALLEL_PARKING;
            break;
            
        case 'R':
            currentState = STATE_PERPENDICULAR_PARKING;
            break;
            
        case 'X':
            currentState = STATE_IDLE;
            stopMotors();
            LED_PARKING_PORT &= ~(1 << LED_PARKING_PIN);
            break;
            
        default:
            break;
    }
}

int main(void) {
    init();
    sendString("Self-Parking Car System Initialized\r\n");
    
    while (1) {
        frontDistance = measureFrontDistance();
        rightDistance = measureRightDistance();
        leftDistance = measureLeftDistance();
        
        switch (currentState) {
            case STATE_IDLE:
                stopMotors();
                break;
                
            case STATE_SEARCHING:
                moveForward(SPEED_SLOW);
                
                if (frontDistance < SAFE_DISTANCE) {
                    stopMotors();
                    currentState = STATE_IDLE;
                    break;
                }
                
                if (detectParkingSlot()) {
                    stopMotors();
                    
                    if (leftDistance > 3 * VEHICLE_WIDTH) {
                        parkingType = 1;
                        currentState = STATE_PERPENDICULAR_PARKING;
                    } else {
                        parkingType = 0;
                        currentState = STATE_PARALLEL_PARKING;
                    }
                }
                break;
                
            case STATE_PARALLEL_PARKING:
                performParallelParking();
                currentState = STATE_PARKED;
                break;
                
            case STATE_PERPENDICULAR_PARKING:
                performPerpendicularParking();
                currentState = STATE_PARKED;
                break;
                
            case STATE_PARKED:
                stopMotors();
                LED_PARKING_PORT ^= (1 << LED_PARKING_PIN);
                _delay_ms(500);
                break;
                
            default:
                currentState = STATE_IDLE;
                break;
        }
        
        _delay_ms(50);
    }
    
    return 0;
}