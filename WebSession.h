/* 
* WebSession.h
*
* Created: 16.08.2014 15:24:02
* Author: Martin
*/


#ifndef __WEBSESSION_H__
#define __WEBSESSION_H__

#include <Arduino.h> 
#include "Global.h"
#include "ConfigData.h"
#include "WebCommand.h"

/*
Session starts with connect cmd. UserId is passed with connect cmd.


*/



/// <summary>
/// Represents a session with a remote device
/// </summary>
class WebSession
{
//variables
public:
	/// <summary>
	/// The session identifier, random number
	/// </summary>
	unsigned long sessionId;

	/// <summary>
	/// The message counter of the session
	/// </summary>
	unsigned long messageIdx;

	/// <summary>
	/// The users level - 
	/// </summary>
	unsigned int sessionMode;

	/// <summary>
	/// The user/device identifier
	/// </summary>
	unsigned int userId;

	/// <summary>
	/// The user password
	/// </summary>
	unsigned int userKey;

protected:
private:
//functions
public:
	WebSession();
	~WebSession();

	/// <summary>
	/// Start a new session.
	/// </summary>
	/// <param name="command">The command - to read the userid.</param>
	/// <returns>The session id, if the user is known - otherwise 0</returns>
	unsigned long newSession(WebCommand *command);

	/// <summary>
	/// Verifies the command. Completes hash calculation and compares the hash with the hash parameter of the command
	/// </summary>
	/// <param name="command">The command to validate.</param>
	/// <returns>True, if the command is valid and the hash value is correct</returns>
	bool verifyCommand(WebCommand *command);

	/// <summary>
	/// Ends this session. Reset session id and other session parameters
	/// </summary>
	void endSession();
protected:
private:
	WebSession( const WebSession &c );
	WebSession& operator=( const WebSession &c );
}; //webSession

extern WebSession webSession;


#endif //__WEBSESSION_H__
