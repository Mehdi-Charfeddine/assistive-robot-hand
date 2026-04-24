#include <Servo.h>
#include "servo_controller.h"
#include "config.h"

ServoController servoController;

void setup() {
    Serial.begin(SERIAL_BAUD);
    
    #if DEBUG_MODE
    Serial.println("Starting Servo-Flex Controller...");
    #endif
    
    servoController.initialize();
    
    #if DEBUG_MODE
    Serial.println("Setup complete!");
    Serial.println("Flex readings format: [flex1, flex2, flex3, flex4, flex5]");
    #endif
}

void loop() {
    // Read all flex sensors and control servos
    for(int i = 0; i < NUM_SERVOS; i++) {
        int flexReading = analogRead(FLEX_PINS[i]);
        int servoPosition = servoController.calculatePosition(i, flexReading);
        servoController.moveServo(i, servoPosition);
    }
    
    #if DEBUG_MODE
    printFlexReadings();
    #endif
    
    delay(LOOP_DELAY);
}

void printFlexReadings() {
    Serial.print("Flex readings: [");
    for(int i = 0; i < NUM_SERVOS; i++) {
        Serial.print(analogRead(FLEX_PINS[i]));
        if(i < NUM_SERVOS - 1) {
            Serial.print(", ");
        }
    }
    Serial.println("]");
}