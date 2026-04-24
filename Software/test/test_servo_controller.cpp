#include <assert.h>
#include <iostream>

// Mock Arduino functions for testing
int map(int value, int fromLow, int fromHigh, int toLow, int toHigh)
{
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

int constrain(int value, int min, int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

// Simple test framework
class TestRunner
{
private:
    int tests_run = 0;
    int tests_passed = 0;

public:
    void assertEqual(int expected, int actual, const char *testName)
    {
        tests_run++;
        if (expected == actual)
        {
            tests_passed++;
            std::cout << "✓ " << testName << " PASSED" << std::endl;
        }
        else
        {
            std::cout << "✗ " << testName << " FAILED - Expected: " << expected
                      << ", Got: " << actual << std::endl;
        }
    }

    void assertTrue(bool condition, const char *testName)
    {
        tests_run++;
        if (condition)
        {
            tests_passed++;
            std::cout << "✓ " << testName << " PASSED" << std::endl;
        }
        else
        {
            std::cout << "✗ " << testName << " FAILED" << std::endl;
        }
    }

    void printResults()
    {
        std::cout << "\n=== Test Results ===" << std::endl;
        std::cout << "Tests run: " << tests_run << std::endl;
        std::cout << "Tests passed: " << tests_passed << std::endl;
        std::cout << "Success rate: " << (100.0 * tests_passed / tests_run) << "%" << std::endl;
    }
};

// Include your config and controller (you'll need to mock Servo class)
class MockServo
{
public:
    int position = 90;
    void attach(int pin) {}
    void write(int pos) { position = pos; }
    int read() { return position; }
};

// Mock the Servo class
#define Servo MockServo

#include "../src/config.h"

// Simplified ServoController for testing
class ServoController
{
private:
    MockServo servos[NUM_SERVOS];
    int currentPositions[NUM_SERVOS];

public:
    void initialize()
    {
        for (int i = 0; i < NUM_SERVOS; i++)
        {
            currentPositions[i] = 90;
        }
    }

    int calculatePosition(int servoIndex, int flexReading)
    {
        if (servoIndex < 0 || servoIndex >= NUM_SERVOS)
        {
            return 90;
        }

        int position = map(flexReading,
                           FLEX_MIN_VALUES[servoIndex],
                           FLEX_MAX_VALUES[servoIndex],
                           SERVO_MIN_ANGLES[servoIndex],
                           SERVO_MAX_ANGLES[servoIndex]);

        return constrain(position, 0, 180);
    }

    bool isValidPosition(int position)
    {
        return (position >= 0 && position <= 180);
    }

    bool isValidServoIndex(int index)
    {
        return (index >= 0 && index < NUM_SERVOS);
    }
};

// Test functions
void testServoPositionCalculation(TestRunner &runner)
{
    ServoController controller;

    // Test servo 0 (inverted: 840-900 maps to 180-0)
    runner.assertEqual(180, controller.calculatePosition(0, 840), "Servo0 Min Reading");
    runner.assertEqual(0, controller.calculatePosition(0, 900), "Servo0 Max Reading");
    runner.assertEqual(90, controller.calculatePosition(0, 870), "Servo0 Mid Reading");

    // Test servo 1 (normal: 879-882 maps to 0-180)
    runner.assertEqual(0, controller.calculatePosition(1, 879), "Servo1 Min Reading");
    runner.assertEqual(180, controller.calculatePosition(1, 882), "Servo1 Max Reading");
}

void testValidationFunctions(TestRunner &runner)
{
    ServoController controller;

    // Test valid positions
    runner.assertTrue(controller.isValidPosition(0), "Position 0 Valid");
    runner.assertTrue(controller.isValidPosition(90), "Position 90 Valid");
    runner.assertTrue(controller.isValidPosition(180), "Position 180 Valid");

    // Test invalid positions
    runner.assertTrue(!controller.isValidPosition(-1), "Position -1 Invalid");
    runner.assertTrue(!controller.isValidPosition(181), "Position 181 Invalid");

    // Test valid servo indices
    runner.assertTrue(controller.isValidServoIndex(0), "Index 0 Valid");
    runner.assertTrue(controller.isValidServoIndex(4), "Index 4 Valid");

    // Test invalid servo indices
    runner.assertTrue(!controller.isValidServoIndex(-1), "Index -1 Invalid");
    runner.assertTrue(!controller.isValidServoIndex(5), "Index 5 Invalid");
}

void testEdgeCases(TestRunner &runner)
{
    ServoController controller;

    // Test out-of-range flex readings
    int result1 = controller.calculatePosition(0, 800); // Below min
    runner.assertTrue(result1 >= 0 && result1 <= 180, "Below range constrained");

    int result2 = controller.calculatePosition(0, 950); // Above max
    runner.assertTrue(result2 >= 0 && result2 <= 180, "Above range constrained");
}

int main()
{
    TestRunner runner;

    std::cout << "Running Servo Controller Tests...\n"
              << std::endl;

    testServoPositionCalculation(runner);
    testValidationFunctions(runner);
    testEdgeCases(runner);

    runner.printResults();

    return 0;
}