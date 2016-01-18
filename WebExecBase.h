// WebExecBase.h

#ifndef _WEBEXECBASE_h
#define _WEBEXECBASE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "WebCommand.h" 

/// <summary>
/// Abstract interface to the WebExec class
/// This class evaluates the command and determines the required action
/// </summary>
class WebExecBase
{
public:
	/// <summary>
	/// Evaluates and executes the command.
	/// </summary>
	/// <param name="cmd">The command.</param>
	virtual void execCommand(WebCommand *cmd) = 0;

	/// <summary>
	/// Sets the required user level to execute the given command.
	/// </summary>
	/// <param name="cmd">The command.</param>
	virtual void setLevelRequired(WebCommand *cmd) = 0;
};


#endif

