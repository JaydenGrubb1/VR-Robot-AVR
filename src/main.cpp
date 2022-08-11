#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define OSC_FREQ 25000000
#define PWM_FREQ 50
#define PWM_MIN 125
#define PWM_MAX 570

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup()
{
	pwm.begin();
	pwm.setOscillatorFrequency(OSC_FREQ);
	pwm.setPWMFreq(PWM_FREQ);

	for (size_t i = 0; i < 16; i++)
	{
		pwm.setPWM(i, 4096, 0);
	}
}

// Servo 0 - no response but significant current draw - dead?
// Servo 1 - good
// Servo 2 - no response
// Servo 3 - sometimes no response - bad wiring?
// Servo 4 - good
// Servo 5 - good
void setServo(int servo, int angle)
{
	int pulse = map(angle, 0, 180, PWM_MIN, PWM_MAX);
	pwm.setPWM(servo, 0, pulse);
}

void loop()
{
	int servo = 7;

	for (size_t i = 1; i < 180; i++)
	{
		setServo(servo, i);
		delay(5);
	}
	delay(100);
	for (size_t i = 180; i > 0; i--)
	{
		setServo(servo, i);
		delay(5);
	}
	delay(100);
}