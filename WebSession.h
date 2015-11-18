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



class WebSession
{
//variables
public:
  unsigned long sessionId;
  unsigned long messageIdx;
  unsigned int sessionMode;

  unsigned int userId;
  unsigned int userKey;
  unsigned int userMode;
protected:
private:
//functions
public:
	WebSession();
	~WebSession();
	unsigned long newSession(WebCommand *command);
	bool verifyCommand(WebCommand *command);
	void endSession();
protected:
private:
	WebSession( const WebSession &c );
	WebSession& operator=( const WebSession &c );
}; //webSession

extern WebSession webSession;


#endif //__WEBSESSION_H__
