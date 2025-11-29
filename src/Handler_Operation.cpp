/**
 * @file 		Operation_Handler.cpp 
 * @brief  	Lezione 2 - Gestione comandi di ESP32  
 */

#include "Config.h"

void ProcessCommand( )
{
	/// Does nothing if empty buffer
	if (bufferOccupation == 0)
		return;

	/// First char in buffer is the code of the specific command
	int cmdCode = command[0];

	if(cmdCode == '\n')
		cmdCode = command[1];

	/// Scans buffer area and parses command
	switch (cmdCode)
	{
		/// Cmd code 'X': marks connection closed by peer 
		case 'X':
			#ifdef USE_WIFI
			clientOfServer.stop();
			#endif
			Serial.println("\nCONNECTION CLOSED BY PEER");
			break;

		case 'O':
			#ifdef USE_LED
			LEDon(LEDPIN);
			#endif
			Serial.println("\nLED ON");
			break;

		case 'Q':
			#ifdef USE_LED
			LEDoff(LEDPIN);
			#endif
			Serial.println("\nLED OFF");
			break;

		default:
			break;
	}

	/// Clears command buffer for future incoming data
	bufferOccupation = 0;
	memset(command, 0, COMMAND_BUFFER_SIZE);
	command[0] = '\0';	
}