#ifndef CONFIG_H
#define CONFIG_H

// Hardware configuration
#define NUM_SERVOS 5
#define LOOP_DELAY 300

// Servo pins
const int SERVO_PINS[NUM_SERVOS] = {2, 3, 4, 5, 6};

// Flex sensor pins
const int FLEX_PINS[NUM_SERVOS] = {A1, A2, A3, A4, A5};

// Calibration data for each flex sensor
const int FLEX_MIN_VALUES[NUM_SERVOS] = {840, 879, 828, 874, 855};
const int FLEX_MAX_VALUES[NUM_SERVOS] = {900, 882, 824, 878, 851};

// Servo angle mappings (some inverted)
const int SERVO_MIN_ANGLES[NUM_SERVOS] = {180, 0, 180, 0, 180};
const int SERVO_MAX_ANGLES[NUM_SERVOS] = {0, 180, 0, 180, 0};

// Debug settings
#define DEBUG_MODE true
#define SERIAL_BAUD 9600

#endif