/*
 * Local_Stepper.h - LocalStepper library for ESP32 & Stepper Motor Driver ST-M5045
 *
 * Original library        (0.1)   by Muhammet Emin Yanik.
 *
 * Drives a NEMA 23 Stepper Motor via Stepper Motor Driver ST-M5045.
 *
 */

// ensure this library description is only included once
#ifndef Local_Stepper_h
#define Local_Stepper_h

// library interface description
class LocalStepper {
  public:
    // constructor:
    LocalStepper(unsigned int number_of_steps, int motor_dir_pin, int motor_pulse_pin);

    // speed setter method:
    void setSpeed(unsigned int speed_rpm);

    // mover method:
    void step(int steps_to_move);

    int version(void);

  private:
    void stepMotor();				// One step command to the motor

    int direction;					// Direction of rotation
    unsigned long step_delay;		// Delay between steps, in us, based on speed
	unsigned long pos_pulse_delay;	// Duration of positive pulse
    unsigned int number_of_steps;	// Total number of steps this motor can take

    // motor pin numbers:
    int motor_dir_pin;
    int motor_pulse_pin;

};

#endif

