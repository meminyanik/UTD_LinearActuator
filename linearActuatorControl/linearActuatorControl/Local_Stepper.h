/*
 * Local_Stepper.h - LocalStepper library for ESP32 & Phase stepper Motor Driver ST-M5045
 */

// ensure this library description is only included once
#ifndef Local_Stepper_h
#define Local_Stepper_h

// library interface description
class LocalStepper {
  public:
    // constructors:
    LocalStepper(int number_of_steps, int motor_dir_pin, int motor_pulse_pin);

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

  private:
    void stepMotor();		// One Step

    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in us, based on speed
    int number_of_steps;      // total number of steps this motor can take

    // motor pin numbers:
    int motor_dir_pin;
    int motor_pulse_pin;

    unsigned long last_step_time; // time stamp in us of when the last step was taken
};

#endif

