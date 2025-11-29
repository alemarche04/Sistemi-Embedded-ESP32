/**
 * @file main.cpp 
 * @brief  Lezione 1  

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
}
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                    LOOP                                    */
/* -------------------------------------------------------------------------- */
void loop() 
{ 
  bool commandPresent = false;

  #ifdef SERVER_MODE
    /// Verifies incoming connections
    WiFiServerCheckNewConnection(); // verifica se un nuovo client è connesso o se il client attuale si è disconnesso

    /// Receives and stores incoming commands if connected
    if ( 	WiFiServerCheckIncomingCommand() )
      commandPresent = true;
  #endif

  #ifdef CLIENT_MODE
    #ifdef USE_BUTTON
    if (ButtonPressed(BUTTONPIN)) // se viene premuto il pulsante viene eseguita la sequenza di comandi come client
    {
      WiFiClientConnectToServer("192.168.1.115", SERVER_PORT); // tentativo di coneessione remoto sulla porta 11777
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
    ProcessCommand( ); // elabora il comando
}
/* -------------------------------------------------------------------------- */