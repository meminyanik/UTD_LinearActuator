/*
* Local_Stepper.h - LocalStepper library for ESP32 & Phase stepper Motor Driver ST-M5045
*/

#include "Arduino.h"
#include "Local_Stepper.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
LocalStepper::LocalStepper(int number_of_steps, int motor_dir_pin, int motor_pulse_pin)
{
  this->direction = 0;      // motor direction
  this->last_step_time = 0; // time stamp in us of the last step taken
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
void LocalStepper::setSpeed(long whatSpeed)
{
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
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
    //unsigned long now = micros();
    // move only if the appropriate delay has passed:
    //if (now - this->last_step_time >= this->step_delay)
    //{
      // get the timeStamp of when you stepped:
     // this->last_step_time = now;
      
	  // step the motor,
	  stepMotor();
      
	  // decrement the steps left:
      steps_left--;
	  
    //}
  }
}

/*
 * Moves the motor forward or backwards.
 */
void LocalStepper::stepMotor()
{
	digitalWrite(motor_pulse_pin, HIGH);
	delayMicroseconds(this->step_delay / 2);
	digitalWrite(motor_pulse_pin, LOW);
	delayMicroseconds(this->step_delay / 2);
}

/*
  version() returns the version of the library:
*/
int LocalStepper::version(void)
{
  return 1;
}
