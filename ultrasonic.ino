#include "pins.h"
#include "ultrasonic.h"
#include <avr/io.h>
#include <util/delay.h>


void timer1_init() {
    TCCR1A = 0;
    TCCR1B = (1 << CS11);  // Prescaler 8 -> 0.5 µs per count (tick)
    TCNT1 = 0;
}

uint16_t timer1_read_us() {
    return TCNT1 >> 1;  // shift right (divide tick count by 2 to get microseconds)
}

uint16_t ultrasonic_read(uint8_t trig_bit, uint8_t echo_bit) {
    // Send 10 µs pulse on TRIG
    CLEAR_BIT(PORTC, trig_bit);
    _delay_us(2);
    SET_BIT(PORTC, trig_bit);
    _delay_us(10);
    CLEAR_BIT(PORTC, trig_bit);

    // Wait for echo to go HIGH
  	TCNT1 = 0;
    while (!READ_BIT(PINC, echo_bit)) {
        if (timer1_read_us() > 30000) return 0;  // 30 ms timeout to avoid infinite loop
    }

    // Measure how long echo stays HIGH
    uint16_t start = timer1_read_us();
    while (READ_BIT(PINC, echo_bit)) {
        if (timer1_read_us() - start > 30000) return 0;
    }

    uint16_t duration = timer1_read_us() - start;
    // Convert to cm (Try different divisors: 58, 57, 56, 55, 54 on tinkercad. 55 was the most suitable divisor)
    return duration / 55;  
}

void ultrasonic_init() {
    timer1_init();
    // Set TRIG pins (PC0, PC2, PC4) as OUTPUT
    SET_BIT(DDRC, PC0);
    SET_BIT(DDRC, PC2);
    SET_BIT(DDRC, PC4);

    // Set ECHO pins (PC1, PC3, PC5) as INPUT
    CLEAR_BIT(DDRC, PC1);
    CLEAR_BIT(DDRC, PC3);
    CLEAR_BIT(DDRC, PC5);
}