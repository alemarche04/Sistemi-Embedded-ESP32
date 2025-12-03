/**
 * @file 	Servo_Handler.cpp 
 * @brief  	Embedded Systems Lab - Handling of servo operations
 */

#include "Config.h"

#ifdef USE_SERVO

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Initializes servo operation
	*/
	bool ServoInit(Servo & servoObj, int servoPin)
	{
		ESP32PWM::allocateTimer(0);
		ESP32PWM::allocateTimer(1);
		ESP32PWM::allocateTimer(2);
		ESP32PWM::allocateTimer(3);

		servoObj.setPeriodHertz(50);    // standard 50 hz servo
		servoObj.attach(servoPin, 500, 2500); // attaches the servo on pin 18 to the servo object
		// using default min/max of 1000us and 2000us
		// different servos may require different min/max settings
		// for an accurate 0 to 180 sweep
		return true;
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Sets servo angle
	*/
	void ServoSetAngle( Servo & servoObj, int angleDeg)
	{
		if(angleDeg < ENDSTOP_LOW)
		{
			angleDeg = ENDSTOP_LOW;
		}
		if(angleDeg > ENDSTOP_HIGH)
		{
			angleDeg = ENDSTOP_HIGH;
		}
		servoObj.write(angleDeg);
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Servo Calibration
	*/
	void ServoTest(Servo & servoObj)
	{
		for(int pos = ENDSTOP_LOW; pos < ENDSTOP_HIGH; pos++)
		{
			ServoSetAngle(servoObj, pos);
			delay(30); // sets movement speed
		}
		delay(3000);
		for(int pos = ENDSTOP_HIGH; pos >= ENDSTOP_LOW; pos--)
		{
			ServoSetAngle(servoObj, pos);
			delay(30); // sets movement speed
		}
		delay(3000);
	}

#endif // USE_Servo