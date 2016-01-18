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
#include "IpSetupBase.h"
#include "WebExecBase.h"
#include "WebCommand.h"

/// <summary>
/// Class to parse inquiries from the web server 
/// Parses incoming URLs, fills the command object 
/// and tries to get the command executed
/// </summary>
class WebParser
{
  public:
    WebParser();
	/// <summary>
	/// Executes this instance. Has to be placed in the loop function of the ino file
	/// </summary>
	void execute();

	/// <summary>
	/// Initializes the web parser and passes required helper objects.
	/// </summary>
	/// <param name="led">The led controller.</param>
	/// <param name="webPages">The web pages output class.</param>
	/// <param name="webExec">The executing object.</param>
	void begin(LedBase *led, WebPages *webPages, WebExecBase *webExec);

	/// <summary>
	/// Fatal error during initialization. Output error message do serial port, set LED to red and stop execution.
	/// </summary>
	/// <param name="msg">The error message.</param>
	void fatalError(char * msg);

	/// <summary>
	/// Parses and executes the command.
	/// Execute immediatedly, if it is an internal command - otherwise have it executed by the <seealso>WebExec</seealso> object
	/// </summary>
	/// <param name="urlPath">The URL path.</param>
	/// <param name="pchUrlTail">The partial URL tail -  after the command keyword.</param>
	void executeCommand(char *urlPath, char *pchUrlTail);
	WebPages *_webPages;
	WebExecBase *_webExec;
	WebSession webSession;
  private:
	  /// <summary>
	  /// Parses the URL tail and reads the parameters.
	  /// </summary>
	  /// <param name="urlPath">The full URL path.</param>
	  /// <param name="pchUrlTail">The URL tail.</param>
	  void readParams(char *urlPath, char *pchUrlTail);

	  /// <summary>
	  /// Executes the administrative set user command.
	  /// </summary>
	  /// <param name="cmd">The command.</param>
	  /// <returns></returns>
	  bool setUser(WebCommand * cmd);

	  /// <summary>
	  /// The led controll instance (for initialization)
	  /// </summary>
	  LedBase *led;

	  /// <summary>
	  /// The command instance
	  /// </summary>
	  WebCommand command;
};

extern WebParser webParser;

#endif

