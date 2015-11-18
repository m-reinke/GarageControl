/* 
* WebSession.cpp
*
* Created: 16.08.2014 15:24:01
* Author: Martin
*/


#include "WebSession.h"
#include <EEPROM.h>
#include <SD.h>
#include <Arduino.h>


// default constructor
WebSession::WebSession()
{
	
} //webSession

// default destructor
WebSession::~WebSession()
{
} //~webSession

bool WebSession::verifyCommand(WebCommand *command)
{
	bool inSession = this->sessionMode >= command->userMode;

	command->calcHash(KEY_SESSIONID, sessionId);
	command->calcHash(KEY_SESSIONIDX, messageIdx);
	command->calcHash(KEY_USERID, userId);
	if (sessionMode > UM_USER)
	{
		command->calcHash(KEY_USERKEY, userKey);
	}

	return inSession && command->verifyHash();
}

void WebSession::endSession()
{
	sessionId = 0;
}

unsigned long WebSession::newSession(WebCommand *command)
{	
	t_user_record user;

	if (configData.loadUser(command->cmd_userId, &user))
	{
		this->userId = userId;
		this->messageIdx = 1;
		this->sessionId = random(1, 0xFFFFFFFF);
		this->sessionMode = user.userMode;

		return this->sessionId;		
	}
	else
	{
		return 0;
	}
}

WebSession webSession;