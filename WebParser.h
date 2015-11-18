 /*
  GarageWeb.h - Library for a Garage opener.
*/
#ifndef GarageWeb_h
#define GarageWeb_h

#include "Arduino.h"
#include "Global.h"

#include <SPI.h>

#ifdef CC3000SHIELD
#include <Adafruit_CC3000.h>
#else
#include <Ethernet.h>
#endif

#include "WebPages.h"
#include "WebSession.h"
#include "Constants.h"
#include "LedBase.h"
#include "WebExecBase.h"
#include "WebCommand.h"



class GarageWeb
{
  public:
    GarageWeb();
    void execute();
	void begin(LedBase *led, WebPages *webPages, WebExecBase *webExec);
    void executeCommand (char *pchUrlTail);
	WebPages *_webPages;
	WebExecBase *_webExec;
	WebSession webSession;
  private:  
	void readParams();

	WebCommand command;
};

extern GarageWeb garageWeb;

#endif

