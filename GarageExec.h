// GarageExec.h

#ifndef _GARAGEEXEC_h
#define _GARAGEEXEC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "WebExecBase.h"
#include "WebCommand.h"
#include "WebPages.h"

class GarageExec : public WebExecBase
{
private:
	WebPages *_webPages;
public:
	void begin(WebPages *webPages);

	virtual void execCommand(WebCommand command);
	virtual void setLevelRequired(WebCommand command);

};

extern GarageExec garageExec;

#endif

