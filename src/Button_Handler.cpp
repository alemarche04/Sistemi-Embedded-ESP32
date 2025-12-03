/**
 * @file 	Button_Handler.cpp 
 * @brief  	Embedded Systems Lab- Handling of button operations
*/

#include "Config.h"

#ifdef USE_BUTTON

/* -------------------------------------------------------------------------- */
/**
 * @brief  Initializes button operation
 */

int  ButtonInit( const int pin )
{
	pinMode( pin, INPUT_PULLUP );
	return 0;
}

/* -------------------------------------------------------------------------- */
/**
 * @brief  Checks button status (NO DEBOUNCING)
 */

bool ButtonPressed( const int pin )
{
	if ( digitalRead(pin) == LOW )
	{
		return true;
	}
	return false;
}

/* -------------------------------------------------------------------------- */
/**
 * @brief  Diagnostic 
 */

bool ButtonTest( int pin )
{
	if (ButtonPressed(pin) )
	{
		Serial.println("Button check OK");

		#ifdef USE_LED
		LEDon(LEDPIN);
		delay(1000);
		LEDoff(LEDPIN);
		#endif // USE_LED
	}
	return true;
}

#endif // USE_BUTTON