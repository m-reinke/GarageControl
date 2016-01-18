
#ifndef WEBDUINO_NO_IMPLEMENTATION
#define WEBDUINO_NO_IMPLEMENTATION
#endif

#include "WebParser.h"


// --------------- Prototypes passed to web server

// root message, returning HTML
void ShowWebRoot(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete);

// message for specific message defined in the tail
void UrlPathCommand(WebServer &server, WebServer::ConnectionType type,
	char **url_path, char *url_tail,
	bool tail_complete);

// Just return page not found
void ShowPageNotFound(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete);


// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
WebServer HttpServer("", 80);


WebParser::WebParser() {
}


void WebParser::begin(LedBase *led, WebPages *webPages, WebExecBase *webExec) {

	this->_webExec = webExec;
	this->_webPages = webPages;
	this->led = led;

	// check coding
	if (this->_webPages == NULL)
	{
		fatalError("Web Pages not initialized");
	}
	if (this->led == NULL)
	{
		fatalError("LED not initialized");
	}

	// Open network interface
	ipInitializer->open(true);

	this->led->SetLed(ColorCyan, ALWAYS_ON);

	// Start http server
	webPages->begin(&HttpServer);

	// initialize http server commands
	HttpServer.setDefaultCommand(ShowWebRoot);
	HttpServer.addCommand("index.html", ShowWebRoot);
	HttpServer.setUrlPathCommand(UrlPathCommand);
	HttpServer.setFailureCommand(ShowPageNotFound);

	led->SetLed(ColorGreen, ALWAYS_ON);

#if WEBDUINO_SERIAL_DEBUGGING > 1
	Serial.println(F("Starting server"));
#endif
	HttpServer.begin();

#if WEBDUINO_SERIAL_DEBUGGING > 1
	Serial.println(F("Listening"));
#endif

	led->AllOff();
}

void WebParser::fatalError(char *msg) {
	Serial.println(msg);
	while (true);
}


void WebParser::executeCommand(char *urlPath, char *pchUrlTail)
{
	readParams(urlPath, pchUrlTail);

	// connect message handeled internally
	if (command.equals(CMD_CONNECT))
	{
#if WEBDUINO_SERIAL_DEBUGGING > 1
		Serial.println(F("*** Connecting ***"));
#endif

		uint32_t sessionId = webSession.newSession(&command);
		if (sessionId > 0)
			_webPages->ConnectMessage(sessionId);
		else
			// error if no session ID generated
			_webPages->ErrorMessage(STATE_NO_USER, "No session id");

		return;
	}
	// user admin messages handeled internally
	else if (command.equals(CMD_USER))
	{
		// always run this in admin mode
		command.userMode = UM_ADMIN;

		if (webSession.verifyCommand(&command))
		{
			setUser(&command);
			_webPages->ResultMessage(STATE_OK);
		}
		else
		{
			_webPages->ErrorMessage(STATE_NO_SESSION, "Verification error");
		}
		return;
	}

	// -- external commands from here --

	// let webExec decide the required user level
	_webExec->setLevelRequired(&command);

	if (!webSession.verifyCommand(&command))
	{
		_webPages->ErrorMessage(STATE_NO_SESSION, "Verification error");
	}

	// now execute the command (and return the reyply from webExec)
	_webExec->execCommand(&command);
}

void WebParser::readParams(char *urlPath, char *pchUrlTail)
{
	// Buffer variables
	const int nMaxParameterBuffer = 25;
	char achName[nMaxParameterBuffer];
	char achValue[nMaxParameterBuffer];

#if WEBDUINO_SERIAL_DEBUGGING > 1
	Serial.print(F("Command \""));
	Serial.print(urlPath);
	Serial.println(F("\""));

	Serial.print(F("Tail: \""));
	Serial.print(pchUrlTail);
	Serial.println(F("\""));
#endif

	// reading parameters always starts a new command
	command.startNewCommand(urlPath);

	bool hasParam = false;
	char *tail = pchUrlTail;

	// this could be used, if runing in POST mode
	//while (HttpServer.readPOSTparam(achName, sizeof(achName), achValue, sizeof(achValue)))

	// Work through all the parameters supplied 
	while (true)
	{
		// let the web server parse for the next param
		URLPARAM_RESULT paramResult = HttpServer.nextURLparam(&tail, achName, sizeof(achName), achValue, sizeof(achValue));
#if WEBDUINO_SERIAL_DEBUGGING > 5
		Serial.print(F("paramResult "));
		Serial.println(paramResult);
#endif

		if (paramResult != URLPARAM_OK)
			break;

#if WEBDUINO_SERIAL_DEBUGGING > 5
		Serial.print(F("param "));
		Serial.print(name);
		Serial.print(F(": \""));
		Serial.print(value);
		Serial.print(F("\""));
#endif

		if (!command.addParam(achName, achValue))
		{
#if WEBDUINO_SERIAL_DEBUGGING > 5
			Serial.println(F(" ignored"));
#endif
			break;
		}
		else
		{
#if WEBDUINO_SERIAL_DEBUGGING > 5
			Serial.println(F(" added"));
#endif
		}
		hasParam = true;
	}

	if (!hasParam)
#if WEBDUINO_SERIAL_DEBUGGING > 1
		Serial.println(F("No parameters"));
#endif
}

bool WebParser::setUser(WebCommand * cmd)
{
	cmd->userMode = UM_ADMIN;

	// extract and convert the parameters
	uint8_t idx = cmd->getIntValue(KEY_USERIDX);

	uint16_t id = cmd->getIntValue(KEY_SETUSERID);
	uint16_t key = cmd->getIntValue(KEY_SETUSERKEY);
	uint16_t mode = cmd->getIntValue(KEY_USERMODE);

	configData.setUser(idx, id, key, mode);

	return true;
}


void WebParser::execute()
{
	char achBuffer[100];
	int nBufferLength = sizeof(achBuffer);

	// let the webserver check, if a new message has arrived
	HttpServer.processConnection(achBuffer, &nBufferLength);
}

// --------------- Page request handlers
void ShowWebRoot(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete)
{
	// We show the password entry page as the web root. Presents a form that asks for a password.
	rServer.httpSuccess();

	if (Type == WebServer::GET)
	{
#if WEBDUINO_SERIAL_DEBUGGING > 1
		Serial.println(F("*** GET ***"));
#endif
		webParser._webPages->IndexPage();
	}
	else
	{
#if WEBDUINO_SERIAL_DEBUGGING > 1
		Serial.println(F("*** Request not supported ***"));
		webParser._webPages->SendPageNotFound();
#endif
	}
}

void UrlPathCommand(WebServer &server, WebServer::ConnectionType type,
	char **url_path, char *url_tail,
	bool tail_complete)
{
	// this is handeled by the webParser
	if (type == WebServer::GET)
	{
		server.httpSuccess("text/xml; charset=utf-8");
		webParser.executeCommand(*url_path, url_tail);
	}
}

void ShowPageNotFound(WebServer &rServer, WebServer::ConnectionType Type, char *pchUrlTail, bool bTailComplete)
{
	// We show the password entry page as the web root. Presents a form that asks for a password.
	HttpServer.httpSuccess();
	webParser._webPages->SendPageNotFound();
}

WebParser webParser;
