#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Servo.h>
#include "config.h"

class ServoController
{
private:
    Servo servos[NUM_SERVOS];
    int currentPositions[NUM_SERVOS];

public:
    // Initialization
    void initialize();

    // Core functionality
    int calculatePosition(int servoIndex, int flexReading);
    void moveServo(int servoIndex, int position);

    // Utility functions
    bool isValidPosition(int position);
    bool isValidServoIndex(int index);

    // Getters for testing
    int getCurrentPosition(int servoIndex);

    // Debug functions
    void printStatus();
};

#endif