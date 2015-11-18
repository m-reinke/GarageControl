/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
  
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include "Global.h"

#include <SPI.h>
#ifdef ETHERSHIELD
#include <Ethernet.h>
#endif 
#ifdef CC3000SHIELD
#include <Adafruit_CC3000.h>
#endif

#include "Arduino.h"

#ifndef WEBDUINO_NO_IMPLEMENTATION
#define WEBDUINO_NO_IMPLEMENTATION
#endif
#include "WebServer.h"

#include "ConfigData.h"
#include "WebSession.h"
#include "GarageSensor.h"
#include "GarageMotor.h"
#include "WebParser.h"
#include "WebPages.h"
#include "Constants.h"
#include "LedBase.h"
#include "SipHash_2_4.h"
#include "HexConversionUtils.h"



// --------------- Prototypes
void ShowWebRoot(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete);
void CommandMessage(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete);
bool IsValidLogin(WebServer &rServer);
void ShowPageNotFound(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete);


// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
WebServer HttpServer("", 80);


GarageWeb::GarageWeb(){
}


void GarageWeb::begin(LedBase *led, WebPages *webPages, WebExecBase *webExec) {
  this->_webPages = webPages;
  this->_webExec = webExec;

  webPages->begin(&HttpServer);

  led->SetLed(ColorYellow, ALWAYS_ON);
  delay(1000);
 
  HttpServer.setDefaultCommand(ShowWebRoot);
  HttpServer.addCommand("command", CommandMessage);
  HttpServer.addCommand("index.html", ShowWebRoot);
  HttpServer.setFailureCommand(ShowPageNotFound);

  // start the Ethernet connection and the server:
//  Ethernet.begin(mac, ip);  
#if WEBDUINO_SERIAL_DEBUGGING > 1
  Serial.println(F("\nInitializing..."));
#endif

#ifdef CC3000SHIELD
  CC3KPrinter = &Serial;

  if (!cc3000.begin())
  {
#if WEBDUINO_SERIAL_DEBUGGING > 1
	  Serial.println(F("Couldn't begin()! Check your wiring?"));
#endif
	  while(1);
  }
  
#if WEBDUINO_SERIAL_DEBUGGING > 1
Serial.println(F("Connecting..."));
#endif
   
#if WEBDUINO_SERIAL_DEBUGGING > 1
Serial.println(configData.ssid);
#endif
    
  if (!cc3000.connectToAP(configData.ssid, configData.wlanKey, WLAN_SECURITY, CONNECT_RETRIES)) {
#if WEBDUINO_SERIAL_DEBUGGING > 1
	  Serial.println(F("Failed!"));
#endif
	  while(1);
  }
  
#if WEBDUINO_SERIAL_DEBUGGING > 1
  Serial.println(F("Connected!"));
#endif
  
  if (configData.useDHCP)
  {  
  
  /* Wait for DHCP to complete */
	#if WEBDUINO_SERIAL_DEBUGGING > 1
	  Serial.println(F("Request DHCP"));
	#endif
	  while (!cc3000.checkDHCP())
	  {
		  delay(100); // ToDo: Insert a DHCP timeout!
	  }
  }
#endif //CC3000SHIELD 
#ifdef ETHERSHIELD

  // connect with configuration
  configData.initIp();

#endif

  led->SetLed(ColorPurple, ALWAYS_ON);
  delay(1000);
  
  
  HttpServer.begin();
  
  led->AllOff();
  delay(1000);
  
}

void GarageWeb::executeCommand(char *pchUrlTail)
{
	readParams();

	if (command.equals(CMD_CONNECT))
	{
#if WEBDUINO_SERIAL_DEBUGGING > 1
		Serial.println(F("*** Connecting ***"));
#endif

		uint32_t sessionId = webSession.newSession(&command);
		if (sessionId > 0)
			_webPages->ConnectMessage(sessionId);
		else
			_webPages->ResultMessage(STATE_NO_SESSION);

		exit;
	}

	_webExec->setLevelRequired(command);

	if (!webSession.verifyCommand(&command))
	{
		_webPages->ResultMessage(STATE_NO_SESSION);
	}

	_webExec->execCommand(command);
}

void GarageWeb::readParams()
{
  const int nMaxParameterBuffer = 20;
  char achName[nMaxParameterBuffer];
  char achValue[nMaxParameterBuffer];

  command.startNewCommand();
 
  // Work through all the parameters supplied 
  while (HttpServer.readPOSTparam(achName, sizeof(achName), achValue, sizeof(achValue)))
  {
	String name (achName);
	String value (achValue);
	#if WEBDUINO_SERIAL_DEBUGGING > 1
	Serial.print(F("param "));
	Serial.println(name);
	Serial.print(F(": "));
	Serial.println(value);
	#endif
     
	command.addParam(name, value);
	
  }

}


void GarageWeb::execute()
{
	char achBuffer[64];
	int nBufferLength = sizeof(achBuffer);

	HttpServer.processConnection(achBuffer, &nBufferLength);
}

// --------------- Page request handlers
void ShowWebRoot(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete)
{
	// We show the password entry page as the web root. Presents a form that asks for a password.
	rServer.httpSuccess();
	
	switch (Type)
	{
		case WebServer::GET:
			#if WEBDUINO_SERIAL_DEBUGGING > 1
			Serial.println(F("*** GET ***"));
			#endif
			garageWeb._webPages->IndexPage();
		break;

		case WebServer::POST:
			#if WEBDUINO_SERIAL_DEBUGGING > 1
			Serial.println(F("*** POST ***"));
			#endif
			garageWeb._webPages->SendErrorPage();
		break;

		// None of these are expected, so we don't respond.
		case WebServer::INVALID:
		case WebServer::HEAD:
		case WebServer::PUT:
		case WebServer::DELETE:
		case WebServer::PATCH:
		default:
		break;
	}
}

void CommandMessage(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete)
{

	if ( Type == WebServer::POST)
	{
			rServer.httpSuccess("text/xml; charset=utf-8");
			garageWeb.executeCommand(pchUrlTail);
	}
}

void ShowPageNotFound(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete)
{
	// We show the password entry page as the web root. Presents a form that asks for a password.
	HttpServer.httpSuccess();

	switch (Type)
	{
		case WebServer::GET:
		case WebServer::POST:
		garageWeb._webPages->SendPageNotFound();
		break;

		// None of these are expected, so we don't respond.
		case WebServer::INVALID:
		case WebServer::HEAD:
		case WebServer::PUT:
		case WebServer::DELETE:
		case WebServer::PATCH:
		default:
		break;
	}
}  

GarageWeb garageWeb;
