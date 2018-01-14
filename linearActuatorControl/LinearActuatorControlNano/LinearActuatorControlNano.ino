/*
This program drives the Linear Actuator for TI-FMCW Radar SAR Implementation

Created 09/27/2017
by Muhammet Emin YANIK
Advisor, Prof. Murat TORLAK
The University of Texas at Dallas
*/

#include <Stepper.h>
#include "ConfigNano.h"
#include "Local_Agenda.h"
#include <SoftwareSerial.h>

// initialize the stepper library
Stepper myVerticalStepper(STEPS_PER_ROTATION, VERTICAL_DIRECTION_PIN, VERTICAL_PULSE_PIN);
Stepper myHorizontalStepper(STEPS_PER_ROTATION, HORIZONTAL_DIRECTION_PIN, HORIZONTAL_PULSE_PIN);

SoftwareSerial serialMatlab(SOFTWARE_SERIAL_RX_PIN, SOFTWARE_SERIAL_TX_PIN); // RX, TX

Agenda scheduler;

String matlabCommand = "";
String stepData = "";

// the setup function runs once when you press reset or power the board
void setup() {
	myVerticalStepper.setSpeed(ROTATION_PER_MINUTE*4);
	myHorizontalStepper.setSpeed(ROTATION_PER_MINUTE*4);

	// initialize the serial ports:
	Serial.begin(SERIAL_MONITOR_SPEED);
	serialMatlab.begin(SERIAL_MATLAB_SPEED);

	serialMatlab.setTimeout(10000);

	scheduler.insert(task_stepper, 20000);
	scheduler.insert(task_matlab, 20000);
}

// the loop function runs over and over again until power down or reset
void loop() {
	scheduler.update();
}


void task_matlab()
{
	while (serialMatlab.available() > 0)
	{
		matlabCommand = serialMatlab.readStringUntil('\n');

		if (matlabCommand.charAt(0) == 'V') {

			isVerticalCommand = true;

			stepData = matlabCommand.substring(1);
			verticalActuatorStep = stepData.toInt();

			Serial.print("Move Actuator Vertical: ");
			Serial.println(verticalActuatorStep);

		}
		else if (matlabCommand.charAt(0) == 'H') {

			isHorizontalCommand = true;

			stepData = matlabCommand.substring(1);
			horizontalActuatorStep = stepData.toInt();

			Serial.print("Move Actuator Horizontal: ");
			Serial.println(horizontalActuatorStep);

		}
		else {
			Serial.println("Wrong Command");
		}
	}

	matlabCommand = "";
	stepData = "";

}

void task_stepper()
{

	if (isVerticalCommand) {
		myVerticalStepper.step(verticalActuatorStep);
		Serial.print("Actuator Vertical Moved: ");
		Serial.println(verticalActuatorStep);
		isVerticalCommand = false;
	}
	else if (isHorizontalCommand) {
		myHorizontalStepper.step(horizontalActuatorStep);
		Serial.print("Actuator Horizontal Moved: ");
		Serial.println(horizontalActuatorStep);
		isHorizontalCommand = false;
	}

}
