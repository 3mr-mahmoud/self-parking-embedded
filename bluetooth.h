// bluetooth.h - Bluetooth control functions for car

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdbool.h>
#include <avr/io.h>

typedef enum {
    STATE_STOPPED,
    STATE_FORWARD,
    STATE_BACKWARD,
    STATE_RIGHT,
    STATE_LEFT,
    STATE_FORWARD_LEFT,
    STATE_FORWARD_RIGHT,
    STATE_BACKWARD_LEFT,
    STATE_BACKWARD_RIGHT
} MovementState;


void bluetooth_init(void);
void process_bluetooth_commands(char cmd);
void set_parking_mode(bool enable);
void set_speed(char speed_level);
bool is_parking_mode_active(void);

// Bluetooth command characters
#define CMD_FORWARD          'F'
#define CMD_BACKWARD         'B'
#define CMD_RIGHT            'R'
#define CMD_LEFT             'L'
#define CMD_FORWARD_LEFT     'G'
#define CMD_FORWARD_RIGHT    'H'
#define CMD_BACKWARD_LEFT    'I'
#define CMD_BACKWARD_RIGHT   'J'
#define CMD_STOP             'S'
#define CMD_HORN             'Y'
#define CMD_HEADLIGHT_ON     'X'
#define CMD_HEADLIGHT_OFF    'x'
#define CMD_SPEED_0          '0'
#define CMD_SPEED_1          '1'
#define CMD_SPEED_2          '2'
#define CMD_SPEED_3          '3'
#define CMD_SPEED_4          '4'
#define CMD_SPEED_5          '5'
#define CMD_SPEED_6          '6'
#define CMD_SPEED_7          '7'
#define CMD_SPEED_8          '8'
#define CMD_SPEED_9          '9'

#endif // BLUETOOTH_H