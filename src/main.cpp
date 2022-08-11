#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define OSC_FREQ 25000000
#define PWM_FREQ 50
#define PWM_MIN 125
#define PWM_MAX 570
#define MOTOR_OFF -1
#define MOTOR_MID -2
#define AXIS_ALL -1

const int AXIS_MIN[] = {-1, -1, 035, -1, 015, 000, 000};
const int AXIS_MAX[] = {-1, -1, 150, -1, 179, 180, 178};

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/**
 * @brief Set the angle of a specified servo
 *
 * @param servo The servo number (port on driver board)
 * @param angle The angle to move to (0-180 deg)
 */
void setServo(int servo, int angle)
{
	if (angle == MOTOR_OFF)
	{
		pwm.setPWM(servo, 4096, 0);
		return;
	}

	int pulse = map(angle, 0, 180, PWM_MIN, PWM_MAX);
	pulse = constrain(pulse, PWM_MIN, PWM_MAX);
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
	if (axis == AXIS_ALL)
	{
		for (size_t i = 0; i < 16; i++)
		{
			setAxis(i, angle);
		}
		return;
	}

	if(angle == MOTOR_MID)
		angle = map(1, 0, 2, AXIS_MIN[axis], AXIS_MAX[axis]);

	if (angle != MOTOR_OFF)
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
 * @brief Returns the nth substring seperated by del
 *
 * @param str The string to search through
 * @param pos Which substring to return
 * @param del The string used as a seperator
 * @return The substring found
 */
String split(String str, int pos, String del)
{
	String search = str;

	while (pos > 0)
	{
		int index = search.indexOf(del);
		if (index == -1)
			return "";
		search = search.substring(index + 1);
		pos--;
	}

	int again = search.indexOf(del);

	if (again == -1)
		return search;
	else
		return search.substring(0, again);
}

/**
 * @brief Parse commands sent over the serial bus
 *
 * @param cmd The command to parse
 */
void parseCmd(String cmd)
{
	String proc = split(cmd, 0, " ");

	if (proc.equals("off"))
	{
		setAxis(AXIS_ALL, MOTOR_OFF);
		return;
	}

	if (proc.equals("axis"))
	{
		int axis = AXIS_ALL;
		String second = split(cmd, 1, " ");

		if (!second.equals("all"))
			axis = second.toInt();

		if (axis == 0)
			if (!second.equals("0"))
				return;

		String third = split(cmd, 2, " ");

		if (third.equals("off"))
		{
			setAxis(axis, MOTOR_OFF);
			return;
		}

		if (third.equals("mid"))
		{
			setAxis(axis, MOTOR_MID);
			return;
		}

		int angle = third.toInt();
		setAxis(axis, angle);
	}
}

/**
 * @brief Initializes robot
 *
 */
void setup()
{
	Serial.begin(9600);

	pwm.begin();
	pwm.setOscillatorFrequency(OSC_FREQ);
	pwm.setPWMFreq(PWM_FREQ);

	setAxis(AXIS_ALL, MOTOR_OFF);
}

/**
 * @brief Main program loop
 *
 */
void loop()
{
	while (Serial.available())
	{
		String str = Serial.readString();
		parseCmd(str);
	}
}

// --------------- MOTOR TEST RESULTS ---------------
// Axis 0 - Servo 0 - no response but significant current draw - dead?
// Axis 1 - Stepper - not tested
// Axis 2 - Servo 1 - good
// Axis 3 - Servo 2 - no response
// Axis 4 - Servo 3 - sometimes no response - bad wiring?
// Axis 5 - Servo 4 - good
// Axis 6 - Servo 5 - good