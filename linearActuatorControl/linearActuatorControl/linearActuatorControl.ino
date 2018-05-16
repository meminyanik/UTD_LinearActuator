/*
This program drives the Linear Actuator for TI-FMCW Radar SAR Implementation

Created by Muhammet Emin YANIK
Advisor, Prof. Murat TORLAK
The University of Texas at Dallas
*/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Local_Stepper.h"
#include <Stepper.h>

#include "Config.h"

// initialize the stepper library
#ifdef LOCAL_STEPPER
	 LocalStepper myVerticalStepper(PULSE_PER_REVOLUTION, VERTICAL_DIRECTION_PIN, VERTICAL_PULSE_PIN);
	 LocalStepper myHorizontalStepper(PULSE_PER_REVOLUTION, HORIZONTAL_DIRECTION_PIN, HORIZONTAL_PULSE_PIN);
#else
	Stepper myVerticalStepper(PULSE_PER_REVOLUTION, VERTICAL_DIRECTION_PIN, VERTICAL_PULSE_PIN);
	Stepper myHorizontalStepper(PULSE_PER_REVOLUTION, HORIZONTAL_DIRECTION_PIN, HORIZONTAL_PULSE_PIN);
#endif

HardwareSerial SerialDebug(2);

// the setup function runs once when you press reset or power the board
void setup() {

	pinMode(VERTICAL_ENABLE_PIN, OUTPUT);
	digitalWrite(VERTICAL_ENABLE_PIN, LOW);
	pinMode(HORIZONTAL_ENABLE_PIN, OUTPUT);
	digitalWrite(HORIZONTAL_ENABLE_PIN, LOW);

	#ifdef LOCAL_STEPPER
		myVerticalStepper.setSpeed(revolutionPerMinute);
		myHorizontalStepper.setSpeed(revolutionPerMinute);
	#else
		myVerticalStepper.setSpeed(revolutionPerMinute*4);
		myHorizontalStepper.setSpeed(revolutionPerMinute*4);
	#endif
	
	// initialize the serial ports:
	SerialDebug.begin(SERIAL_DEBUG_SPEED);

	//Processor 0 Tasks
	xTaskCreatePinnedToCore(task_stepper_horizontal, "task_stepper_horizontal", 2048, NULL, 10, NULL, 0);
	xTaskCreatePinnedToCore(task_stepper_continuous, "task_stepper_continuous", 2048, NULL, 10, NULL, 0);

	//Processor 1 Tasks
	xTaskCreatePinnedToCore(task_stepper_vertical, "task_stepper_vertical", 2048, NULL, 10, NULL, 1);
	xTaskCreatePinnedToCore(task_matlab, "task_matlab", 2048, NULL, 10, NULL, 1);
}

// the loop function runs over and over again until power down or reset
void loop() {
	vTaskSuspend(NULL);
}


void task_matlab(void *parameter)
{
	String matlabCommand;
	String parsedData;

	Serial.begin(SERIAL_MATLAB_SPEED);
	
	while (true)
	{

		while (Serial.available() > 0)
		{
			matlabCommand = Serial.readStringUntil('\n');

			if (matlabCommand.charAt(0) == 'S') {

				parsedData = matlabCommand.substring(1);
				revolutionPerMinute = parsedData.toInt();

				#ifdef LOCAL_STEPPER
					myVerticalStepper.setSpeed(revolutionPerMinute);
					myHorizontalStepper.setSpeed(revolutionPerMinute);
				#else
					myVerticalStepper.setSpeed(revolutionPerMinute * 4);
					myHorizontalStepper.setSpeed(revolutionPerMinute * 4);
				#endif

				Serial.print("Revolution Per Minute is Set as: ");
				Serial.println(revolutionPerMinute);

				//SerialDebug.print("Revolution Per Minute is Set as: ");
				//SerialDebug.println(revolutionPerMinute);

			}
			else if (matlabCommand.charAt(0) == 'V') {
				
				parsedData = matlabCommand.substring(1);
				verticalActuatorStep = parsedData.toInt();

				isVerticalCommand = true;
				
				Serial.print("Move Actuator Vertical: ");
				Serial.println(verticalActuatorStep);

				//SerialDebug.print("Move Actuator Vertical: ");
				//SerialDebug.println(verticalActuatorStep);

			}
			else if (matlabCommand.charAt(0) == 'H') {

				parsedData = matlabCommand.substring(1);
				horizontalActuatorStep = parsedData.toInt();

				isHorizontalCommand = true;
				
				Serial.print("Move Actuator Horizontal: ");
				Serial.println(horizontalActuatorStep);

				//SerialDebug.print("Move Actuator Horizontal: ");
				//SerialDebug.println(horizontalActuatorStep);

			}
			else if (matlabCommand.startsWith("CMODE")) {

				isContinousCommand = true;

				Serial.println("Continuous Mode is started");

			}

			else if (matlabCommand.startsWith("CH")) {

				parsedData = matlabCommand.substring(2);
				horizontalActuatorStep = parsedData.toInt();
				Serial.println("Continuous Mode Horizontal Pulse command received");

			}

			else if (matlabCommand.startsWith("CV")) {

				parsedData = matlabCommand.substring(2);
				verticalActuatorStep = parsedData.toInt();
				Serial.println("Continuous Mode Vertical Pulse command received");

			}

			else if (matlabCommand.startsWith("CM")) {

				parsedData = matlabCommand.substring(2);
				verticalMeasurement = parsedData.toInt();
				Serial.println("Continuous Mode Vertical Measurement command received");

			}

			else {
				SerialDebug.println("Wrong Command");
			}

		}

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
	return;
}

void task_stepper_horizontal(void *parameter)
{

	while (true)
	{

		if (isHorizontalCommand) {
			digitalWrite(HORIZONTAL_ENABLE_PIN, HIGH);

			int tStamp = micros();

			#ifdef LOCAL_STEPPER
				myHorizontalStepper.step(horizontalActuatorStep);
			#else
				myHorizontalStepper.step(horizontalActuatorStep * 4);
			#endif

			tStamp = micros() - tStamp;

			Serial.print("Number of Horizontal Steps: ");
			Serial.println(abs(horizontalActuatorStep));
			Serial.print("Horizontal Time: ");
			Serial.println(tStamp);

			digitalWrite(HORIZONTAL_ENABLE_PIN, LOW);

			isHorizontalCommand = false;

		}	

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
	return;
}

void task_stepper_vertical(void *parameter)
{

	while (true)
	{
		
		if (isVerticalCommand) {
			digitalWrite(VERTICAL_ENABLE_PIN, HIGH);

			int tStamp = micros();

			#ifdef LOCAL_STEPPER
				myVerticalStepper.step(verticalActuatorStep);
			#else
				myVerticalStepper.step(verticalActuatorStep * 4);
			#endif
			
			tStamp = micros() - tStamp;

			Serial.print("Number of Vertical Steps: ");
			Serial.println(abs(verticalActuatorStep));
			Serial.print("Vertical Time: ");
			Serial.println(tStamp);

			digitalWrite(VERTICAL_ENABLE_PIN, LOW);

			isVerticalCommand = false;
			
		}

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
	return;
}

void task_stepper_continuous(void *parameter)
{

	while (true)
	{

		if (isContinousCommand) {
			digitalWrite(HORIZONTAL_ENABLE_PIN, HIGH);
			digitalWrite(VERTICAL_ENABLE_PIN, HIGH);

			for (int n = 0; n < verticalMeasurement; n++) {

				#ifdef LOCAL_STEPPER
					myHorizontalStepper.step(horizontalActuatorStep);
				#else
					myHorizontalStepper.step(horizontalActuatorStep * 4);
				#endif

				horizontalActuatorStep = -1 * horizontalActuatorStep;

				if (n != verticalMeasurement - 1) {
					#ifdef LOCAL_STEPPER
						myVerticalStepper.step(verticalActuatorStep);
					#else
						myVerticalStepper.step(verticalActuatorStep * 4);
					#endif
				}

			}

			digitalWrite(HORIZONTAL_ENABLE_PIN, LOW);
			digitalWrite(VERTICAL_ENABLE_PIN, LOW);

			isContinousCommand = false;

		}

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
	return;
}
