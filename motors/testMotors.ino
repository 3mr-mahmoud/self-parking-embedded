#include <avr/io.h>
#include "./pins.h"
#include "./motors.h"

void testMotorFunctions() {
  // Initialize motors
  motorInit();
  
  // Test forward movement
  moveForward(SPEED_NORMAL);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test backward movement
  moveBackward(SPEED_NORMAL);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test right turn (in place)
  turnRight(SPEED_SLOW);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test left turn (in place)
  turnLeft(SPEED_SLOW);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test right turn while moving forward
  turnRightForward(SPEED_NORMAL);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test left turn while moving forward
  turnLeftForward(SPEED_NORMAL);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test clockwise rotation
  rotateClockwise(SPEED_SLOW);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test counter-clockwise rotation
  rotateCounterClockwise(SPEED_SLOW);
  delay(2000);
  stopMotors();
  delay(500);
  
  // Test speed variations
  for (uint8_t i = 0; i < 3; i++) {
    uint8_t speed = (i == 0) ? SPEED_SLOW : (i == 1) ? SPEED_NORMAL : SPEED_MAX;
    
    moveForward(speed);
    delay(1000);
    stopMotors();
    delay(300);
    
    moveBackward(speed);
    delay(1000);
    stopMotors();
    delay(300);
  }
  
  // Test individual motor control
  setMotorSpeed(SPEED_NORMAL, 0, 0, 0);  // Only front right
  delay(1000);
  setMotorSpeed(0, SPEED_NORMAL, 0, 0);  // Only front left
  delay(1000);
  setMotorSpeed(0, 0, SPEED_NORMAL, 0);  // Only rear right
  delay(1000);
  setMotorSpeed(0, 0, 0, SPEED_NORMAL);  // Only rear left
  delay(1000);
  stopMotors();
}

void setup() {
  // Set status LED as output
  pinMode(LED_STATUS_PIN, OUTPUT);
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Motor Test Initialized");
}

void loop() {
  // Blink LED to indicate test start
  digitalWrite(LED_STATUS_PIN, !digitalRead(LED_STATUS_PIN));
  
  // Run motor tests
  testMotorFunctions();
  
  // Print to serial monitor
  Serial.println("Test cycle completed");
  
  // Delay before repeating tests
  delay(3000);
}