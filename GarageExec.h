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

/// <summary>
/// Garage opener specific part of a web command system
/// Executing a WebCommand and replying to the sender
/// </summary>
/// <seealso cref="WebExecBase" />
class GarageExec : public WebExecBase
{
private:
	WebPages *_webPages;
public:
	/// <summary>
	/// Initialization - place in the setup function of the ino file.
	/// </summary>
	/// <param name="webPages">The web pages.</param>
	void begin(WebPages *webPages);

	/// <summary>
	/// Execute method - place in the  the command.
	/// </summary>
	/// <param name="command">The command.</param>
	virtual void execCommand(WebCommand *command);

	/// <summary>
	/// Sets the level required.
	/// </summary>
	/// <param name="cod">The cod.</param>
	virtual void setLevelRequired(WebCommand *cod);

};

extern GarageExec garageExec;

#endif

