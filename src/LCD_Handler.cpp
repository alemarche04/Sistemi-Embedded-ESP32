/**
 * @file 	LCD_Handler.cpp 
 * @brief  	Embedded Systems Lab- Handling of LCD monitor operations
 */

#include "Config.h"

#ifdef USE_LCD

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Initializes LCD monitor operation
	*/
	bool LCDInit(int numRows, int numCols)
	{
		lcd.init(); // initialize the lcd 
		lcd.backlight(); // print a message to the LCD
		
		return true;
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Writes a message to the LCD monitor
	*/
	void LCDWrite(int row, int col, const char * message)
	{
		LCDSetCursor(row, col);
  		lcd.print(message);
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Clears the LCD monitor
	*/
	void LCDClear()
	{
		lcd.clear();
		lcd.home();
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Sets cursor on LCD monitor
	*/
	void LCDSetCursor(int row, int col)
	{
		lcd.setCursor(col, row);
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Sets cursor on LCD monitor
	*/
	void LCDTest()
	{
		LCDClear();
		LCDWrite(0, 0, "LCD active");
	}

#endif // USE_LCD