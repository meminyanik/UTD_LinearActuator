#pragma once

// change this to fit the number of steps per revolution for the motor
#define		STEPS_PER_ROTATION				3200

// set the speed at 60 rpm:
#define		ROTATION_PER_MINUTE				60

#define		VERTICAL_DIRECTION_PIN			2
#define		VERTICAL_PULSE_PIN				3

#define		HORIZONTAL_DIRECTION_PIN		4
#define		HORIZONTAL_PULSE_PIN			5

#define		SOFTWARE_SERIAL_RX_PIN			6
#define		SOFTWARE_SERIAL_TX_PIN			7

#define		SERIAL_MONITOR_SPEED			115200
#define		SERIAL_MATLAB_SPEED				115200

bool		isVerticalCommand = false;
long int	verticalActuatorStep;

bool		isHorizontalCommand = false;
long int	horizontalActuatorStep;