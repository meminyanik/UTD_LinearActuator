/*
* Local_Stepper.h - LocalStepper library for ESP32 & Stepper Motor Driver ST-M5045
*
* Original library        (0.1)   by Muhammet Emin Yanik.
*
* Drives a NEMA 23 Stepper Motor via Stepper Motor Driver ST-M5045.
*
*/

#include <Arduino.h>
#include "Local_Stepper.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
LocalStepper::LocalStepper(unsigned int number_of_steps, int motor_dir_pin, int motor_pulse_pin)
{
  this->direction = 0;      // motor direction
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_dir_pin = motor_dir_pin;
  this->motor_pulse_pin = motor_pulse_pin;

  // setup the pins on the microcontroller:
  pinMode(this->motor_dir_pin, OUTPUT);
  pinMode(this->motor_pulse_pin, OUTPUT);

}

/*
 * Sets the speed in revs per minute
 */
void LocalStepper::setSpeed(unsigned int speed_rpm)
{
  this->step_delay = 60 * 1000 * 1000 / this->number_of_steps / speed_rpm;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void LocalStepper::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { this->direction = 1; }
  if (steps_to_move < 0) { this->direction = 0; }

  if (this->direction == 1) { digitalWrite(motor_dir_pin, LOW); }
  else { digitalWrite(motor_dir_pin, HIGH); }

  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {   
	  // step the motor,
	  stepMotor();
      
	  // decrement the steps left:
      steps_left--;
	  
  }
}

/*
 * Moves the motor forward or backwards.
 */
void LocalStepper::stepMotor()
{
	pos_pulse_delay = this->step_delay / 2;

	digitalWrite(motor_pulse_pin, HIGH);
	delayMicroseconds(pos_pulse_delay - 1); // -1 is added based on measurements, can be tuned better
	digitalWrite(motor_pulse_pin, LOW);
	delayMicroseconds(this->step_delay - pos_pulse_delay - 1); // -1 is added based on measurements, can be tuned better
}

/*
  version() returns the version of the library:
*/
int LocalStepper::version(void)
{
  return 1;
}
