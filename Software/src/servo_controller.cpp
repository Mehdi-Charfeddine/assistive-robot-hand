#include "servo_controller.h"

void ServoController::initialize()
{
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        servos[i].attach(SERVO_PINS[i]);
        currentPositions[i] = 90; // Start at middle position
        servos[i].write(90);
    }

#if DEBUG_MODE
    Serial.println("ServoController initialized");
#endif

    delay(1000); // Allow servos to reach initial position
}

int ServoController::calculatePosition(int servoIndex, int flexReading)
{
    if (!isValidServoIndex(servoIndex))
    {
        return 90; // Default to middle position
    }

    // Map flex reading to servo angle
    int position = map(flexReading,
                       FLEX_MIN_VALUES[servoIndex],
                       FLEX_MAX_VALUES[servoIndex],
                       SERVO_MIN_ANGLES[servoIndex],
                       SERVO_MAX_ANGLES[servoIndex]);

    // Constrain to valid servo range
    position = constrain(position, 0, 180);

    return position;
}

void ServoController::moveServo(int servoIndex, int position)
{
    if (!isValidServoIndex(servoIndex) || !isValidPosition(position))
    {
        return;
    }

    servos[servoIndex].write(position);
    currentPositions[servoIndex] = position;

#if DEBUG_MODE
    Serial.print("Servo ");
    Serial.print(servoIndex);
    Serial.print(" moved to ");
    Serial.println(position);
#endif
}

bool ServoController::isValidPosition(int position)
{
    return (position >= 0 && position <= 180);
}

bool ServoController::isValidServoIndex(int index)
{
    return (index >= 0 && index < NUM_SERVOS);
}

int ServoController::getCurrentPosition(int servoIndex)
{
    if (isValidServoIndex(servoIndex))
    {
        return currentPositions[servoIndex];
    }
    return -1; // Invalid index
}

void ServoController::printStatus()
{
    Serial.println("=== Servo Status ===");
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        Serial.print("Servo ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(currentPositions[i]);
    }
    Serial.println("==================");
}