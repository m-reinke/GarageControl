// WebExecBase.h

#ifndef _WEBEXECBASE_h
#define _WEBEXECBASE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "WebCommand.h" 

class WebExecBase
{
public:
	virtual void execCommand(WebCommand cmd) = 0;
	virtual void setLevelRequired(WebCommand cmd) = 0;
};


#endif

