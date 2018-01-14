#pragma once

// change this to fit the number of steps per revolution for the motor
#define		PULSE_PER_REVOLUTION			3200

#define		VERTICAL_DIRECTION_PIN			25
#define		VERTICAL_PULSE_PIN				26

#define		HORIZONTAL_DIRECTION_PIN		32
#define		HORIZONTAL_PULSE_PIN			33

#define		SERIAL_DEBUG_SPEED				115200
#define		SERIAL_MATLAB_SPEED				115200

// set the speed at 60 rpm:
unsigned int	revolutionPerMinute = 60; // Values are between 0-65535.

bool		isVerticalCommand = false;
unsigned int	verticalActuatorStep; // Max step is 51200

bool		isHorizontalCommand = false;
unsigned int	horizontalActuatorStep; // Max step is 51200