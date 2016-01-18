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
	if (_garageSensor != NULL)
	{
		_webServer->print((_garageSensor->GetDoorStateText()));
	}
	else
	{
		_webServer->print(F(" unknown - Sensor not present"));
	}
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

void WebPages::SendMessageFooter()
{
	// same footer for HTML and XML
	_webServer->print(F("</html>"));
}

void WebPages::StatusMessage()
{
	String key;
	if (_garageSensor != NULL)
	{
		key = _garageSensor->GetDoorStateKey();
	}
	else
	{
		key = "MIS";
	}

	SingleValueMessage(KEY_STATE, key);
}

void WebPages::ConnectMessage(uint32_t sessionId)
{
	String sSessionId = String(sessionId);
	SingleValueMessage(KEY_SESSIONID, sSessionId);
}

void WebPages::ResultMessage(String result)
{
	SendMessageHeader();
	SendResultValue(result);
	SendMessageFooter();
}

void WebPages::ErrorMessage(String result, String error)
{
	SendMessageHeader();
	SendErrorValue(result, error);
	SendMessageFooter();
}

void WebPages::SingleValueMessage(String key, String value)
{
	SendMessageHeader();
	SendResultValue(STATE_OK);
	SendKeyValue(key, value);
	SendMessageFooter();
}

void WebPages::SendKeyValue(String key, String value)
{
	//<DATA ID="key">value</DATA>
	_webServer->print(F("<"));
	_webServer->print(F(KEY_DATA));

	_webServer->print(F(" "));

	_webServer->print(ATTR_ID);
	_webServer->print(F("=\""));
	_webServer->print(key);
	_webServer->print(F("\" "));

	_webServer->print(F(">"));

	_webServer->print(value);

	_webServer->print(F("</"));
	_webServer->print(F(KEY_DATA));
	_webServer->print(F(">"));
}

void WebPages::SendResultValue(String result)
{
	//<RESULT STATUS="result" />
	_webServer->print(F("<"));
	_webServer->print(F(KEY_RESULT));
	_webServer->print(F(" "));
	_webServer->print(F(ATTR_STATE));
	_webServer->print(F("=\""));
	_webServer->print(result);
	_webServer->print(F("\" />"));
}

void WebPages::SendErrorValue(String result, String error)
{
	//<RESULT STATUS="result">error</RESULT>

	_webServer->print(F("<"));
	_webServer->print(F(KEY_RESULT));
	_webServer->print(F(" "));
	_webServer->print(F(ATTR_STATE));
	_webServer->print(F("=\""));
	_webServer->print(result);
	_webServer->print(F("\" >"));

	_webServer->print(error);

	_webServer->print(F("</"));
	_webServer->print(F(KEY_DATA));
	_webServer->print(F(">"));
}

WebPages webPages;

