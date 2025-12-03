/**
 * @file 	Config.h 
 * @brief   Embedded Systems Lab- Configuration file
*/

#include <Arduino.h>

#undef   	USE_LED
#define  	USE_LED

#undef   	USE_BUTTON
#define  	USE_BUTTON

#undef		USE_LCD
#define		USE_LCD

#undef 		USE_POT
#define 	USE_POT

#undef		USE_SERVO
#define		USE_SERVO

#undef		USE_US
#define		USE_US

#define 	USE_SERIAL
#undef 		USE_SERIAL

#define 	USE_WIFI
#undef 		USE_WIFI

#ifdef USE_WIFI
	#define		SERVER_MODE
	#define   	CLIENT_MODE
#else
	#undef		SERVER_MODE
	#undef    	CLIENT_MODE
#endif


/* -------------------------------------------------------------------------- */
/*                               COMMON SECTION                               */
/* -------------------------------------------------------------------------- */
#define 			END_COMMAND_MARKER				'$'
#define				COMMAND_BUFFER_SIZE				32
extern char 		command[COMMAND_BUFFER_SIZE];
extern int 			bufferOccupation;	
void 				ProcessCommand();
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                 LED SECTION                                */
/* -------------------------------------------------------------------------- */
#ifdef USE_LED

	int 	LEDInit(int pin);
	void	LEDon(int pin);
	void  	LEDoff(int pin);
	void 	LEDflash(int pin, int durationSec, int timelapMs = 1000);
	bool	LEDTest(int pin);

#endif // USE_LED
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                               /BUTTON SECTION                              */
/* -------------------------------------------------------------------------- */
#ifdef USE_BUTTON

	int  ButtonInit(const int pin);
	bool ButtonPressed(const int pin);
	bool ButtonTest(int pin);

#endif // USE_BUTTON
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                 LCD SECTION                                */
/* -------------------------------------------------------------------------- */
#ifdef USE_LCD

	#include <Wire.h>
	#include <LiquidCrystal_I2C.h>

	#define NUM_ROWS	2
	#define NUM_COLS	16

	extern LiquidCrystal_I2C lcd; // global variable to set the LCD address

	bool LCDInit(int numRows, int numCols);
	void LCDWrite(int row, int col, const char * message);
	void LCDClear();
	void LCDSetCursor(int row, int col);
	void LCDTest();

#endif // USE_LCD
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                SERVO SECTION                               */
/* -------------------------------------------------------------------------- */
#ifdef USE_SERVO

	#include<ESP32Servo.h>
	#define ENDSTOP_LOW 	5
	#define ENDSTOP_HIGH 	175

	extern Servo demoServo;

	bool ServoInit(Servo &, int servoPin);
	void ServoSetAngle(Servo &, int angleDeg);
	void ServoTest(Servo &);

#endif // USE_SERVO
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                              US SENSOR SECTION                             */
/* -------------------------------------------------------------------------- */
#ifdef USE_US

	#define TRIGGERPIN
	#define ECHOPIN
	#define SOUNDSPEED		0.0017150 // [cm_us]

	bool USInit(int trigPin, int echoPin);
	float UDdistanceCm(int trigPin, int echoPin, float speed = SOUNDSPEED);

#endif // USE_US
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                               SERIAL SECTION                               */
/* -------------------------------------------------------------------------- */
#ifdef USE_SERIAL

	#define RXD2 27
	#define TXD2 14

	bool SerialInit();
	bool SerialCheckIncomingCommand();
	bool SerialTransmit(const char* message);

#endif // USE_SERIAL
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                WIFI  SECTION                               */
/* -------------------------------------------------------------------------- */
#ifdef USE_WIFI

	#include <WiFi.h>
	#include <WiFiAP.h>
	#include <WiFiMulti.h>
	#include "arduino_secrets.h"

	#define 	INVALID_WIFI_MODE			0
	#define		AP_MODE						1
	#define		EXT_WIFI_CLIENT_MODE		2

	extern int					wifiMode;
	extern WiFiMulti 			multiHandler;
	extern WiFiClient     		clientOfServer, clientToRemoteServer;
	extern bool 				serverConnected, clientConnected;

	#ifdef SERVER_MODE
		#define		SERVER_PORT		11777
		extern 		WiFiServer 		server;
	#endif
	#ifdef CLIENT_MODE
		#define 	NUM_RECONNECTION_TRIALS		5
		#define		REMOTE_SERVER_PORT			11777
	#endif

	bool WiFiInit(int wifiMod);
	bool WiFiConnectToAP(const char * ssid = 0, const char * password = 0);
	bool WiFiInitAP(const char * ssid, const char * password);

	/// @brief  Server op
	void WiFiServerStart();
	void WiFiServerShutdown();
	bool WiFiServerCheckNewConnection();
	bool WiFiServerCheckIncomingCommand();
	
	/// @brief  Client op
	bool WiFiClientConnectToServer(const char * serverIP, int serverPortNum);
	void WiFiClientDisconnectFromServer();
	bool WiFiClientTransmitToServer(String message);

	bool WiFiTest();

#endif // USE_WIFI
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                               PIN DEFINITIONS                              */
/* -------------------------------------------------------------------------- */

#ifdef 		USE_LED
#define 	LEDPIN 		5					/// Status LED pin
#endif 
#ifdef 		USE_BUTTON
#define 	BUTTONPIN 	26					/// Interface button pin
#endif 
#ifdef 		USE_POT
#define 	POTPIN 		25					/// Potentiometer pin
#endif 
#ifdef 		USE_SERVO
#define 	SERVOPIN 	33					/// Servo pin
#endif 

/* -------------------------------------------------------------------------- */