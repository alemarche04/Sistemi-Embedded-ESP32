/**
 * @file    main.cpp 
 * @brief   Embedded Systems Lab
*/

#include "Config.h"


/* -------------------------------------------------------------------------- */
/*                            GLOBAL VARIABLE DEFS                            */
/* -------------------------------------------------------------------------- */

int 		bufferOccupation = 0;	
char    command[COMMAND_BUFFER_SIZE];

#ifdef USE_WIFI	
  int	          wifiMode      = INVALID_WIFI_MODE;
  WiFiMulti     multiHandler;
  WiFiServer 		server( SERVER_PORT );
  WiFiClient    clientOfServer, clientToRemoteServer;

  bool 					serverConnected = false, clientConnected = false;
#endif

#ifdef USE_LCD
  LiquidCrystal_I2C lcd(0x27, NUM_COLS, NUM_ROWS); // set the LCD address to 0x27 for a 16 chars and 2 line display
#endif //USE_LCD	int 	LEDInit(int pin);

#ifdef USE_SERVO
  Servo demoServo;
#endif // USE_SERVO
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                    SETUP                                   */
/* -------------------------------------------------------------------------- */
void setup() 
{
  Serial.begin( 9600 ); // inizializzazione comunicazione seriale

  /* -------------------------------- LED Init -------------------------------- */
  #ifdef USE_LED
    if ( LEDInit( LEDPIN ) == 0)
    {
      Serial.println("LED Init ....................... OK");
    } 
    else 
    {
      Serial.println("LED Init ....................... FAILED");
    }
  #endif // USE_LED

  /* -------------------------------- LCD INIT -------------------------------- */
  #ifdef USE_LCD
    Serial.println("LCD Init ....................... OK");
    LCDInit(NUM_ROWS, NUM_COLS);

    LCDTest(); delay(2000); LCDClear();
  #endif // USE_LCD

  /* ------------------------------- BUTTON Init ------------------------------ */
  #ifdef USE_BUTTON
    if ( ButtonInit( BUTTONPIN ) == 0)
    {
      Serial.println("Button Init .................... OK");
    } 
    else 
    {
      Serial.println("Button Init .................... FAILED");
    }
  #endif // USE_BUTTON

  /* ------------------------------- SERVO Init ------------------------------- */
  #ifdef USE_SERVO
    Serial.println("Servo Init .................... OK");
    ServoInit(demoServo, SERVOPIN);
  #endif // USE_SERVO

  /* ------------------------------- SERIAL Init ------------------------------ */
  #ifdef USE_SERIAL
    Serial.println("Serial Init .................... OK");
    SerialInit();
  #endif // USE_SERIAL

  /* -------------------------------- WiFi Init ------------------------------- */
  #ifdef USE_WIFI

    bufferOccupation = 0;
    memset(command, 0, COMMAND_BUFFER_SIZE);
    command[0] = '\0';

    bool wifiOK = WiFiInit( EXT_WIFI_CLIENT_MODE ); // configura l'ESP32 come client wifi esterno
    /// Connects to external wifi if required
    if (wifiOK && wifiMode == EXT_WIFI_CLIENT_MODE)
    {
      wifiOK = WiFiConnectToAP( ); // se wifinit() ha successo tenta la connessione all'AP
    }
    if ( wifiOK ) // se è riuscito a connettersi all'AP
    {
      Serial.println("WiFi Init ...................... OK");
      #ifdef USE_LED
      LEDflash(LED_BUILTIN, 2, 1000);
      #endif
    } 
    else // la wifiinit() non è andata a buon fine
    {
      Serial.println("WiFi Init ...................... FAILED");
      #ifdef USE_LED
      LEDflash(LED_BUILTIN, 2, 100);
      #endif
    }
    #ifdef SERVER_MODE // se server mode è attivo
      /// Starts server listening
      WiFiServerStart(); // mette il dispositivo in ascolto sulla porta 11777
      Serial.print("Server Init .................... OK ");
      Serial.println(" (" + String(SERVER_PORT) +")");
    #endif
  #endif // USE_WIFI

  /* ------------------------------- Servo Test ------------------------------- */
  // ServoTest(demoServo);
}
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                    LOOP                                    */
/* -------------------------------------------------------------------------- */
void loop() 
{ 
  //////////
  //return;
  //////////

  bool commandPresent = false;
  uint16_t potValue;

  #ifdef USE_US
    if(CheckAlarmUS(10))
    {

    }
  #endif // USE_US

  
/* ---------- read potentiometer value and prints on serial monitor --------- */
  potValue = analogRead(POTPIN);
  Serial.println(potValue);


/* -------------------- writes potentiometer value on LCD ------------------- */
  #ifdef USE_LCD
    LCDWrite(0, 0, "    ");
    LCDWrite(0, 0, String(potValue).c_str());
  #endif // USE_LCD
  delay(100);


/* -------------- moves servo according to potentiometer value -------------- */
  #ifdef USE_SERVO
    int angle = map(potValue, 0, 4096, ENDSTOP_LOW, ENDSTOP_HIGH);
    ServoSetAngle(demoServo, angle);
    #ifdef USE_LCD
      LCDWrite(0, 7, "   ");
      LCDWrite(0, 7, String(angle).c_str());
    #endif // USE_LCD
  #endif // USE_SERVO


/* ------------ Sends and recieves message with serial connection ----------- */
  #ifdef USE_SERIAL
    /// Receives and stores incoming commands if connected
    if (SerialCheckIncomingCommand())
      commandPresent = true;

    #ifdef USE_BUTTON
      if(ButtonPressed(BUTTONPIN))
      {
        SerialTransmit("O$");
        delay(2000);
        SerialTransmit("Q$");
        delay(2000);
      }
    #endif // USE_BUTTON

  #endif



/* ------------ Sends and recieves message with Wi-Fi connection ------------ */
  #ifdef SERVER_MODE
    /// Verifies incoming connections
    WiFiServerCheckNewConnection(); // verifica se un nuovo client è connesso o se il client attuale si è disconnesso

    /// Receives and stores incoming commands if connected
    if ( WiFiServerCheckIncomingCommand())
      commandPresent = true;
  #endif

  #ifdef CLIENT_MODE
    #ifdef USE_BUTTON
    if (ButtonPressed(BUTTONPIN)) // se viene premuto il pulsante viene eseguita la sequenza di comandi come client
    {
      WiFiClientConnectToServer("192.168.1.115", SERVER_PORT); // tentativo di coneessione remoto sulla porta 11777ifdef USE_LC
      if (clientConnected) // se riesce a connettersi
      {
        WiFiClientTransmitToServer( "O$" ); // comando LED ON
        delay(1000);
        WiFiClientTransmitToServer( "Q$" ); // comando LED OFF
        delay(1000);
        WiFiClientDisconnectFromServer();
      }
    }
    #endif
  #endif

  
  /// Possibly parses received command
  if (commandPresent) // se è stato ricevuto un comando
    ProcessCommand(); // elabora il comando
}
/* -------------------------------------------------------------------------- */