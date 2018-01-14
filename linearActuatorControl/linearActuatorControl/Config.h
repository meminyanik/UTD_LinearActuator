#pragma once

// change this to fit the number of steps per revolution for the motor
#define		STEPS_PER_ROTATION				3200

// set the speed at 60 rpm:
#define		ROTATION_PER_MINUTE				60

#define		VERTICAL_DIRECTION_PIN			25
#define		VERTICAL_PULSE_PIN				26

#define		HORIZONTAL_DIRECTION_PIN		32
#define		HORIZONTAL_PULSE_PIN			33

#define		SERIAL_DEBUG_SPEED				115200
#define		SERIAL_MATLAB_SPEED				115200

bool		isVerticalCommand = false;
long int	verticalActuatorStep;

bool		isHorizontalCommand = false;
long int	horizontalActuatorStep;