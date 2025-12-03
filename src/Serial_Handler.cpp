/**
* @file 	Serial_Handler.cpp 
* @brief  	Embedded Systems Lab - Handling of serial communications
*/

#include "Config.h"

#ifdef USE_SERIAL

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Initializes serial operation
	*/
	bool SerialInit()
	{
		Serial.begin(9600), SERIAL_8N1, RXD2, TXD2; 
		// 9600 = Baud Rate
		// SERIAL_8N1 = 8 data bits, No parity, and 1 stop bit
		return true;
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Checks for incoming messages
	*/
	bool SerialCheckIncomingCommand()
	{
		char c;

		while (Serial.available())  // checks if there are new thing in rx buffer
		{     
			// if there's bytes to read from the client,
			c = Serial.read();     // read a byte, then
			Serial.write(c);

			/// Checks buffer overflow
			if (bufferOccupation >= COMMAND_BUFFER_SIZE)
				bufferOccupation = 0;
			/// Stores incoming char in command buffer
			command[bufferOccupation] = c;
			bufferOccupation++;


			/// Checks if last char indicated end-of-command
			if (c == END_COMMAND_MARKER)
			{
				Serial.println("  --> Command terminator detected");
				
				return true;			
			}
			/// Command is only marked present if a end-marker has been found  
			return false;
		}
		return false; 
	}

/* -------------------------------------------------------------------------- */
	/**
	* @brief  Transmit message
	*/
	bool SerialTransmit(const char* message)
	{
		// param validation
		if(!message || strlen(message) == 0)
			return false;
		
		// debug
		Serial.print("Sending byte sequence = ");
		for(int k = 0; k < strlen(message); k++)
		{
			Serial.print(message[k]);
			Serial.print(" ");
		}
		Serial.println();

		while(Serial.availableForWrite() == 0); // returns true if I can write

		// transmit individual bytes
		int count = 0;
		for(int k = 0; k < strlen(message); k++)
		{
			count += Serial.write(message[k]); // conta i caratteri
		}
		
		if(count != strlen(message)) // controlla il numero di caratteri mandati
			return false;
		
		return true;
	}

#endif // USE_SERIAL