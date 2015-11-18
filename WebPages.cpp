#include "WebPages.h"

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

WebPages::WebPages ()
{
}

void WebPages::begin(SensorBase *garageSensor)
{
	_garageSensor = garageSensor;
}

void WebPages::begin(WebServer *ws)
{
	_webServer = ws;
}

void WebPages::SendHeader()
{
	_webServer->print(F("<html><head><title>Garage Door</title>"));
//	_webServer->print(F("<meta name='viewport' content='width=320; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;'/>"));
	_webServer->print(F("</head>"));
}


void WebPages::IndexPage ()
{
#if WEBDUINO_SERIAL_DEBUGGING > 1
Serial.println(F("*** Index Page ***"));
#endif
	SendHeader();

//	_webServer->print(F("Refresh: 5"));  // refresh the page automatically every 5 sec 
	_webServer->print(F("Garage is "));
	_webServer->print((_garageSensor->GetDoorStateText()));
	_webServer->print(F("<br />"));
	_webServer->print(F("</html>"));
}

void WebPages::SendAccessDeniedPage()
{
	SendHeader();
	_webServer->println(F("<body>"));
	_webServer->print(F("<p>None shall pass!</p>"));
	_webServer->print(F("</body></html>"));
}

void WebPages::SendErrorPage()
{
	SendHeader();
	_webServer->println(F("<body><p>Something has gone wrong. Please try later</p></body></html>"));
}

void WebPages::SendPageNotFound()
{
	SendHeader();
	_webServer->println(F("<body><p>Can't find that here</p></body></html>"));
}

//------------------------------------------------------------------------------------------------

void WebPages::SendMessageHeader()
{
	_webServer->print(F("<html><head>"));
	_webServer->print(F("<title>Garage Door</title>"));
	_webServer->print(F("</head>"));
}


void WebPages::StatusMessage()
{
	SingleValueMessage(KEY_STATE, _garageSensor->GetDoorStateKey());
}

void WebPages::ConnectMessage(uint32_t sessionId)
{
	SingleValueMessage(KEY_SESSIONID, String(sessionId));
}

void WebPages::ResultMessage(String result)
{
	SingleValueMessage(KEY_RESULT, result);
}

void WebPages::SingleValueMessage(String key, String value)
{
	SendMessageHeader();
	_webServer->print(F("<div ID=\""));
	_webServer->print(key);
	_webServer->print(F("\">"));
	_webServer->print(value);
	_webServer->print(F("</div>"));
	_webServer->print(F("</html>"));
}

WebPages webPages;

/*
void WebServer::print( char * f )
{
	
}*/
