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

 /// <summary>
 /// Web- and XML pages returned by the garage opener
 /// </summary>
class WebPages
{
	public:
		WebPages();

		/// <summary>
		/// Initialize and pass an instance of a garage sensor.
		/// </summary>
		/// <param name="garageSensor">The garage sensor.</param>
		void begin(SensorBase *garageSensor);

		/// <summary>
		/// Initialize and pass an instance of a garage sensor.
		/// </summary>
		/// <param name="ws">The web server instance.</param>
		void begin(WebServer * ws);

		/// <summary>
		/// Return the index page (HTML).
		/// </summary>
		void IndexPage();

		/// <summary>
		/// Sends the access denied page (HTML).
		/// </summary>
		void SendAccessDeniedPage();

		/// <summary>
		/// Sends a unspecific error page (HTML).
		/// </summary>
		void SendErrorPage();

		/// <summary>
		/// Sends a page not found reply (HTML).
		/// </summary>
		void SendPageNotFound();

		/// <summary>
		/// Sends a garage state message (XML).
		/// </summary>
		void StatusMessage();

		/// <summary>
		/// Sends a message with an execution result (XML).
		/// </summary>
		/// <param name="result">The result.</param>
		void ResultMessage(String result);

		/// <summary>
		/// Sends a specific error message.
		/// </summary>
		/// <param name="result">The error result key.</param>
		/// <param name="error">The human readable error.</param>
		void ErrorMessage(String result, String error);

		/// <summary>
		/// Sends a reply for the connect command.
		/// </summary>
		/// <param name="sessionId">A session identifier ID.</param>
		void ConnectMessage(uint32_t sessionId);

		/// <summary>
		/// Sends a reply message with a single value parameter.
		/// </summary>
		/// <param name="key">The output param key name.</param>
		/// <param name="value">The output value.</param>
		void SingleValueMessage(String key, String value);
	private:
		/// <summary>
		/// Sends the header of a message (HTML).
		/// </summary>
		void SendHeader();

		/// <summary>
		/// Sends the header of a message (XML).
		/// </summary>
		void SendMessageHeader();

		/// <summary>
		/// Sends the header of a message (XML/HTML).
		/// </summary>
		void SendMessageFooter();


		/// <summary>
		/// Sends a key-value pair.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="value">The value.</param>
		void SendKeyValue(String key, String value);

		/// <summary>
		/// Sends the result-status part of the message.
		/// </summary>
		/// <param name="result">The result.</param>
		void SendResultValue(String result);

		/// <summary>
		/// Sends the result-status of a non successfull message.
		/// </summary>
		/// <param name="result">The result.</param>
		/// <param name="error">The error.</param>
		void SendErrorValue(String result, String error);

		/// <summary>
		/// The web server instance used
		/// </summary>
		WebServer *_webServer;

		/// <summary>
		/// The garage sensor for the <seealso>StatusMessage</seealso>
		/// </summary>
		SensorBase *_garageSensor;
};

extern WebPages webPages;

#endif
