/**
 * @file 		LED_Handler.cpp 
 * @brief  	Embedded Systems Lab- Handling of LED operations
 */

#include "Config.h"

#ifdef USE_LED

/* -------------------------------------------------------------------------- */
/**
 * @brief Initialization of LED on designated pin
*/ 
int 	LEDInit( int pin )
{
  /// Definisco pin
  pinMode( LEDPIN, OUTPUT);  

  digitalWrite( LEDPIN, LOW );

  return 0;
}

/* -------------------------------------------------------------------------- */
/**
 * @brief Turns on LED on designated pin
 */ 
void	LEDon( int pin )
{
  digitalWrite( pin, HIGH );
}

/* -------------------------------------------------------------------------- */
/**
 * @brief Turns off LED on designated pin
 */ 
void  LEDoff( int pin )
{
    digitalWrite( pin, LOW );
}

/* -------------------------------------------------------------------------- */
/**
 * @brief Flashes LED on designated pin
 */ 
void LEDflash(int pin, int durationSec, int timelapMs )
{
  digitalWrite( pin, HIGH);
  delay(timelapMs);
  digitalWrite( pin, LOW );
  delay(timelapMs);
}

/* -------------------------------------------------------------------------- */
/**
 * @brief Diagnostic of LED on designated pin
*/ 
bool	LEDTest( int pin )
{
  LEDflash(pin, 1, 300);
  return true;
}

#endif // USE_LED