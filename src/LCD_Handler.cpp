/**
 * @file 	LCD_Handler.cpp 
 * @brief  	Lezione 1 - Gestione LCD
 */

#include "Config.h"

#ifdef USE_LCD

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Initializes button operation
	*/
	bool LCD_Init(int numRows, int numCols)
	{
		lcd.init(); // initialize the lcd 
		lcd.backlight(); // print a message to the LCD
		
		return true;
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Writes a message to the LCD monitor
	*/
	void LCD_Write(int row, int col, const char * message)
	{
		LCD_SetCursor(row, col);
  		lcd.print(message);
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Clears the LCD monitor
	*/
	void LCD_Clear()
	{
		lcd.clear();
		lcd.home();
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Sets cursor on LCD monitor
	*/
	void LCD_SetCursor(int row, int col)
	{
		lcd.setCursor(col, row);
	}

#endif // USE_LCD