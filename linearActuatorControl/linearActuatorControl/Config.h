/*
This program drives the Linear Actuator for TI-FMCW Radar SAR Implementation

Created by Muhammet Emin YANIK
Advisor, Prof. Murat TORLAK
The University of Texas at Dallas
*/

#pragma once

// Comment out following definition if Arduino Stepper Library will be used
#define		LOCAL_STEPPER

// change this to fit the number of steps per revolution for the motor
#define		PULSE_PER_REVOLUTION			5000

#define		DISTANCE_PER_REVOLUTION			5  // in mm

#define		VERTICAL_DIRECTION_PIN			25
#define		VERTICAL_PULSE_PIN				26
#define		VERTICAL_ENABLE_PIN				12

#define		HORIZONTAL_DIRECTION_PIN		32
#define		HORIZONTAL_PULSE_PIN			33
#define		HORIZONTAL_ENABLE_PIN			13

#define		SERIAL_DEBUG_SPEED				115200
#define		SERIAL_MATLAB_SPEED				115200

// set the speed at 30 rpm:
unsigned int	revolutionPerMinute = 30;

bool isVerticalCommand = false;
int verticalActuatorStep;

bool isHorizontalCommand = false;
int horizontalActuatorStep;

bool isContinousCommand = false;
unsigned int verticalMeasurement;