/**
* @file 	Serial_Handler.cpp 
* @brief  	Embedded Systems Lab - Handling of US sensor operations
*/

#include "Config.h"

#ifdef USE_US

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Initializes US sensor operation
	*/
	bool USInit(int trigPin, int echoPin)
	{
		return true;
	}


/* -------------------------------------------------------------------------- */
	/**
	* @brief  Calculates distange from object
	*/
	float UDdistanceCm(int trigPin, int echoPin, float speed = SOUNDSPEED)
	{
		unsigned long timeFlightUs;

		/// Initialize trigger
		digitalWrite(trigPin, LOW);
		delay(100);

		/// Activate trigger
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(50);
		digitalWrite(trigPin, LOW);
		
		timeFlightUs = pulseIn(echoPin, HIGH, 100000UL);

		if(timeFlightUs == 0)
		{
			
		}

	}

#endif // USE_US