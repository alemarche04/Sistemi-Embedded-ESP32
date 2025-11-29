/**
 * @file 		LED_Handler.cpp 
 * @brief  	Lezione 1 - Gestione LED  
 */

#include "Config.h"

#ifdef USE_LED

/////////////////////////////////////////////////////
/**
 * @brief Inizializza LED sul pin designato
*/ 
int 	LEDInit( int pin )
{
  /// Definisco pin
  pinMode( LEDPIN, OUTPUT);  

  digitalWrite( LEDPIN, LOW );

  return 0;
}

/////////////////////////////////////////////////////
/**
 * @brief Accende  LED sul pin designato
 */ 
void	LEDon( int pin )
{
  digitalWrite( pin, HIGH );
}

/////////////////////////////////////////////////////
/**
 * @brief Spegne LED sul pin designato
 */ 
void  LEDoff( int pin )
{
    digitalWrite( pin, LOW );
}

/////////////////////////////////////////////////////
/**
 * @brief Lampeggia LED sul pin designato
 */ 
void LEDflash(int pin, int durationSec, int timelapMs )
{
  digitalWrite( pin, HIGH);
  delay(timelapMs);
  digitalWrite( pin, LOW );
  delay(timelapMs);
}

/////////////////////////////////////////////////////
/// Diagnostic LED sul pin designato

bool	LEDTest( int pin )
{
  LEDflash(pin, 1, 300);
  return true;
}

#endif // USE_LED