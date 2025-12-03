/**
* @file 	WiFi_Handler.cpp 
* @brief  	LEmbedded Systems Lab - Handling of Wi-Fi communications
*/

#include "Config.h"

#ifdef USE_WIFI


/* -------------------------------------------------------------------------- */
/**
* @brief  Checks for incoming connections to server
*/
bool WiFiServerCheckIncomingCommand()
{
	char c;

	if (!serverConnected) // il server non è connesso
		return false;

	if (!clientOfServer.available()) // il client of server non ha nessun dato disponibile
		return false;

	while (clientOfServer.available())  // fino a quando il client ha qualcosa da dire
	{     
		// if there's bytes to read from the client,
		c = clientOfServer.read();     // read a byte, then
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
* @brief  Client operation: transmit data to remote server
*/

bool WiFiClientTransmitToServer( String message )
{
	if (message.length() == 0)
		return false;
	
	if (!clientConnected)
		return false;

	int numBytes = clientToRemoteServer.write( message.c_str() );
	if (numBytes != message.length() )
	{
		Serial.println("Error sending message " + message);
		return false;
	}
	return true;
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Initializes WiFi device operation
*/
bool  WiFiInit( int wifiMod ) // inizializza il dispositivo come AP oppure come Client
{
	/* ------------------------------ AP mode Init ------------------------------ */
	if (wifiMod == AP_MODE) // inizializza il dispositivo come Acces Point
	{
		Serial.println();
		Serial.println("Configuring access point...");		
		bool apOK = WiFiInitAP( WIFI_AP_SSID, WIFI_AP_PWD );
		if (apOK) // se la configurazione come AP è andata a buon fine
		{
			IPAddress myIP = WiFi.softAPIP(); // ottiene l'indirizzo IP di se stesso
			Serial.print("AP IP address: ");
			Serial.println(myIP);		
			wifiMode = AP_MODE; // aggiorna la wifi mode come AP
		}
		else // la configurazione come AP non è andata a buon fine
		{
			Serial.println("AP Init Failed");
			wifiMode = INVALID_WIFI_MODE;
			return false;
		}
  }
	/* ------------------------ EXTERNAL WIFI  mode Init ------------------------ */
	else if (wifiMod == EXT_WIFI_CLIENT_MODE) // modalità Client
	{
		int numAP = 0;

		/// Insert several options for external WiFi
		if (! multiHandler.addAP( MAIN_WIFI_SSID, MAIN_WIFI_PWD) )
			Serial.println("Warning: failed to add AP " + String(MAIN_WIFI_SSID) );
		else
			numAP++;
		if (! multiHandler.addAP( SECONDARY_WIFI_SSID, SECONDARY_WIFI_PWD ) )
			Serial.println("Warning: failed to add AP " + String(SECONDARY_WIFI_SSID) );
		else
			numAP++;

		if (numAP ==0)
		{
			Serial.println("No valid AP has been added: WiFI disabled");
			wifiMode = INVALID_WIFI_MODE;
			return false;
		}
		wifiMode = EXT_WIFI_CLIENT_MODE;
	}
	else
	{
		Serial.println("Invalid Wifi mode " + String(wifiMod));
		return false;
	}
	return true;
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Connects to external WiFi network
*/
bool WiFiConnectToAP( const char * ssid, const char * password )
{
	Serial.println();
	Serial.print("Connecting to WiFi ... ");

	while (multiHandler.run() != WL_CONNECTED) 
	{
		Serial.print(".");
		delay(500);
	}
	Serial.println("OK -- WiFi connected");
	Serial.print("--->  IP address: ");
	Serial.println(WiFi.localIP());

	#ifdef USE_LCD
		LCD_SetCursor(1, 0);
		LCD_Write(1, 0, "               ");
		LCD_Write(1, 0, WiFi.localIP().toString().c_str());
	#endif // USE_LCD

	return true;
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Initializes AccessPoint operation
*/
bool WiFiInitAP( const char * ssid, const char * password ) // inizializza le operazioni di AP
{
	if (!WiFi.softAP(ssid, password)) // funzione di libreria che configura il dispositivo come AP
	{
		wifiMode = INVALID_WIFI_MODE;
		return false;
	}
	return true;
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Starts server operation
*/
void WiFiServerStart( ) // avvia l'operazione di server
{
	server.begin();
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Closes server operation
*/
void WiFiServerShutdown( ) // chiude l'operazione di server
{
	server.stop();
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Checks for incoming connections to server
*/
bool WiFiServerCheckNewConnection() // gestisce lo stato della connessione del client al server
{
	/// If server is connected, checks if connection still valid
	if (serverConnected)
	{
		if (clientOfServer.connected())
		{
			return false;
		}
		Serial.println("\n Remote Client has disconnected");
		bufferOccupation = 0; // Clears buffer of commands that were issued by the dead connection
		serverConnected = false;
	}

	// arrivo qui solo se non sono connesso

	/// If server is not connected, checks if new connection is incoming
	clientOfServer = server.accept();  // listen for incoming clients (true if there are client, false if not; does not wait for any connection)
	// server.accept() returns client address (if it founds a connection)
	if (clientOfServer)
	{
    	Serial.println("\n--> Connection detected from remote client at port " + String(SERVER_PORT));
		serverConnected = true;
	}
	return serverConnected;
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Client operation: connection to remote server
*/

bool WiFiClientConnectToServer( const char * serverIP, int serverPortNum ) // tenta di connettersi a un server specificato
{
  Serial.print("\nConnecting to ");
  Serial.println(serverIP);

	for (int trial=0; trial < NUM_RECONNECTION_TRIALS; trial++)
	{
		Serial.print(".");
		if (clientToRemoteServer.connect( serverIP, serverPortNum )) 
		{
			Serial.println("\nConnection established to server at " + String(serverIP) + " port " + String(serverPortNum));
			clientConnected = true;
			break;
		}
		delay(1000);
	}
	if (!clientConnected)
	{
		Serial.println("\nConnection failed.");
		return false;
	}
	return clientConnected;
}


/* -------------------------------------------------------------------------- */
/**
* @brief  Client operation: disconnection from  remote server
*/

void WiFiClientDisconnectFromServer( ) // disconnette l'ESP dal server remoto
{
	if (clientConnected)
	{
		/// Signals disconnection and closes operation
		WiFiClientTransmitToServer( "X$" );
		delay(50);
		clientToRemoteServer.stop();
	}
	clientConnected = false;
}

#endif // USE_WIFI