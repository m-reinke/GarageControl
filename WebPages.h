/*
 * WebPages.h
 *
 * Created: 04.01.2014 19:45:34
 *  Author: Martin
 */ 

#ifndef WebPages_h
#define WebPages_h

#include "Constants.h"

#ifndef WEBDUINO_NO_IMPLEMENTATION
  #define WEBDUINO_NO_IMPLEMENTATION
#endif

#include "WebServer.h"
#include "SensorBase.h"

class WebPages
{
	public:
		WebPages();
		void begin(SensorBase *garageSensor);
		void begin(WebServer * ws);
		void IndexPage ();
		void SendAccessDeniedPage();
		void SendErrorPage();
		void SendPageNotFound();

		void StatusMessage();
		void ResultMessage(String result);
		void ConnectMessage(uint32_t sessionId);
		void SingleValueMessage(String key, String value);		
	private:
		void SendHeader();
		void SendMessageHeader();
		WebServer *_webServer;
		SensorBase *_garageSensor;
};

extern WebPages webPages;

#endif
