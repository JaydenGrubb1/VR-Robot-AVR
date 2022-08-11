#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define OSC_FREQ 25000000
#define PWM_FREQ 50
#define PWM_MIN 125
#define PWM_MAX 570
#define SERVO_OFF -1

const int AXIS_MIN[] = {000, 000, 000, 000, 000, 000, 000};
const int AXIS_MAX[] = {180, 180, 180, 180, 180, 180, 180};

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/**
 * @brief Set the angle of a specified servo
 *
 * @param servo The servo number (port on driver board)
 * @param angle The angle to move to (0-180 deg)
 */
void setServo(int servo, int angle)
{
	if (angle == SERVO_OFF)
	{
		pwm.setPWM(servo, 4096, 0);
		return;
	}

	int pulse = map(angle, 0, 180, PWM_MIN, PWM_MAX);
	pwm.setPWM(servo, 0, pulse);
}

/**
 * @brief Set the angle of the stepper motor
 *
 * @param angle The angle to move to
 */
void setStepper(int angle)
{
	// TODO Implement this function
}

/**
 * @brief Set the angle of a specified axis
 *
 * @param axis The axis number
 * @param angle The angle to move to (0-180 deg)
 */
void setAxis(int axis, int angle)
{
	angle = constrain(angle, AXIS_MIN[axis], AXIS_MAX[axis]);

	if (axis == 0)
	{
		setServo(0, angle);
		return;
	}

	if (axis == 1)
	{
		setStepper(angle);
		return;
	}

	setServo(axis - 1, angle);
}

/**
 * @brief Initializes robot
 *
 */
void setup()
{
	pwm.begin();
	pwm.setOscillatorFrequency(OSC_FREQ);
	pwm.setPWMFreq(PWM_FREQ);

	for (size_t i = 0; i < 16; i++)
	{
		setServo(i, SERVO_OFF);
	}
}

/**
 * @brief Main program loop
 *
 */
void loop()
{
	setAxis(5, 0);
}

// --------------- SEROV TEST RESULTS ---------------
// Servo 0 - no response but significant current draw - dead?
// Servo 1 - good
// Servo 2 - no response
// Servo 3 - sometimes no response - bad wiring?
// Servo 4 - good
// Servo 5 - good