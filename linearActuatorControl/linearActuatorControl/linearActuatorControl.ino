/*
This program drives the Linear Actuator for TI-FMCW Radar SAR Implementation

Created 09/27/2017
by Muhammet Emin YANIK
Advisor, Prof. Murat TORLAK
The University of Texas at Dallas
*/

#include <Stepper.h>
#include "Config.h"

// initialize the stepper library
Stepper myVerticalStepper(PULSE_PER_REVOLUTION, VERTICAL_DIRECTION_PIN, VERTICAL_PULSE_PIN);
Stepper myHorizontalStepper(PULSE_PER_REVOLUTION, HORIZONTAL_DIRECTION_PIN, HORIZONTAL_PULSE_PIN);
HardwareSerial SerialDebug(2);


// the setup function runs once when you press reset or power the board
void setup() {
	myVerticalStepper.setSpeed(revolutionPerMinute*4);
	myHorizontalStepper.setSpeed(revolutionPerMinute*4);
	
	// initialize the serial ports:
	SerialDebug.begin(SERIAL_DEBUG_SPEED);

	xTaskCreatePinnedToCore(task_stepper_horizontal, "task_stepper", 2048, NULL, 10, NULL, 1);
	xTaskCreatePinnedToCore(task_stepper_vertical, "task_stepper", 2048, NULL, 10, NULL, 1);
	xTaskCreatePinnedToCore(task_matlab, "task_matlab", 2048, NULL, 10, NULL, 1);
}

// the loop function runs over and over again until power down or reset
void loop() {
	vTaskSuspend(NULL);
}


void task_matlab(void * parameter)
{
	String matlabCommand;
	String stepData;
	String rpmData;

	Serial.begin(SERIAL_MATLAB_SPEED);
	
	while (true)
	{

		while (Serial.available() > 0)
		{
			matlabCommand = Serial.readStringUntil('\n');

			if (matlabCommand.charAt(0) == 'S') {

				rpmData = matlabCommand.substring(1);
				revolutionPerMinute = rpmData.toInt();

				myVerticalStepper.setSpeed(revolutionPerMinute * 4);
				myHorizontalStepper.setSpeed(revolutionPerMinute * 4);

				SerialDebug.print("Revolution Per Minute is Set as: ");
				SerialDebug.println(revolutionPerMinute);

			}

			if (matlabCommand.charAt(0) == 'V') {
				
				isVerticalCommand = true;
				
				stepData = matlabCommand.substring(1);
				verticalActuatorStep = stepData.toInt();
				
				SerialDebug.print("Move Actuator Vertical: ");
				SerialDebug.println(verticalActuatorStep);

			}
			else if (matlabCommand.charAt(0) == 'H') {

				isHorizontalCommand = true;

				stepData = matlabCommand.substring(1);
				horizontalActuatorStep = stepData.toInt();
				
				SerialDebug.print("Move Actuator Horizontal: ");
				SerialDebug.println(horizontalActuatorStep);

			}
			else {
				SerialDebug.println("Wrong Command");
			}

		}

		delay(20);
	}
	vTaskDelete(NULL);
	return;
}

void task_stepper_horizontal(void * parameter)
{

	while (true)
	{

		if (isHorizontalCommand) {
			myHorizontalStepper.step(horizontalActuatorStep*4);
			SerialDebug.print("Actuator Horizontal Moved: ");
			SerialDebug.println(horizontalActuatorStep);
			isHorizontalCommand = false;
		}	

		delay(20);
	}
	vTaskDelete(NULL);
	return;
}

void task_stepper_vertical(void * parameter)
{

	while (true)
	{

		if (isVerticalCommand) {
		myVerticalStepper.step(verticalActuatorStep*4);
		SerialDebug.print("Actuator Vertical Moved: ");
		SerialDebug.println(verticalActuatorStep);
		isVerticalCommand = false;
		}

		delay(20);
	}
	vTaskDelete(NULL);
	return;
}

