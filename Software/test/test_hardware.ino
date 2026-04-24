#include <Servo.h>
#include "../src/servo_controller.h"
#include "../src/config.h"

ServoController testController;
bool testsPassed = true;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("=== Hardware Integration Tests ===");
    
    testController.initialize();
    delay(2000);
    
    runHardwareTests();
    
    if (testsPassed) {
        Serial.println("\n✓ All hardware tests PASSED!");
    } else {
        Serial.println("\n✗ Some hardware tests FAILED!");
    }
}

void loop() {
    // Run continuous monitoring test
    if (millis() % 5000 == 0) {
        testFlexSensorReadings();
    }
}

void runHardwareTests() {
    testServoMovement();
    testFlexSensorReadings();
    testSystemIntegration();
}

void testServoMovement() {
    Serial.println("\n--- Testing Servo Movement ---");
    
    for(int i = 0; i < NUM_SERVOS; i++) {
        Serial.print("Testing Servo ");
        Serial.println(i);
        
        // Test positions: 0, 90, 180
        int testPositions[] = {0, 90, 180};
        
        for(int j = 0; j < 3; j++) {
            testController.moveServo(i, testPositions[j]);
            delay(1000);
            
            int currentPos = testController.getCurrentPosition(i);
            if(currentPos == testPositions[j]) {
                Serial.print("  ✓ Position ");
                Serial.print(testPositions[j]);
                Serial.println(" OK");
            } else {
                Serial.print("  ✗ Position ");
                Serial.print(testPositions[j]);
                Serial.println(" FAILED");
                testsPassed = false;
            }
        }
    }
    
    // Return to neutral position
    for(int i = 0; i < NUM_SERVOS; i++) {
        testController.moveServo(i, 90);
    }
}

void testFlexSensorReadings() {
    Serial.println("\n--- Testing Flex Sensor Readings ---");
    
    for(int i = 0; i < NUM_SERVOS; i++) {
        int reading = analogRead(FLEX_PINS[i]);
        
        Serial.print("Flex Sensor ");
        Serial.print(i);
        Serial.print(" (A");
        Serial.print(FLEX_PINS[i] - A0);
        Serial.print("): ");
        Serial.print(reading);
        
        // Check if reading is within expected range (±50 from calibrated values)
        int minExpected = FLEX_MIN_VALUES[i] - 50;
        int maxExpected = FLEX_MAX_VALUES[i] + 50;
        
        if(reading >= minExpected && reading <= maxExpected) {
            Serial.println(" ✓ OK");
        } else {
            Serial.print(" ✗ OUT OF RANGE (expected ");
            Serial.print(minExpected);
            Serial.print("-");
            Serial.print(maxExpected);
            Serial.println(")");
            testsPassed = false;
        }
    }
}

void testSystemIntegration() {
    Serial.println("\n--- Testing System Integration ---");
    
    for(int i = 0; i < NUM_SERVOS; i++) {
        int flexReading = analogRead(FLEX_PINS[i]);
        int calculatedPosition = testController.calculatePosition(i, flexReading);
        
        Serial.print("Servo ");
        Serial.print(i);
        Serial.print(": Flex=");
        Serial.print(flexReading);
        Serial.print(" -> Servo=");
        Serial.print(calculatedPosition);
        
        if(calculatedPosition >= 0 && calculatedPosition <= 180) {
            Serial.println(" ✓ OK");
            testController.moveServo(i, calculatedPosition);
        } else {
            Serial.println(" ✗ INVALID POSITION");
            testsPassed = false;
        }
    }
    
    delay(2000);
}