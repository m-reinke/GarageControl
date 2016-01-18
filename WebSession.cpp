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
	// First check required user level
	bool inSession = this->sessionMode >= command->userMode;

	// always calc hash of the three follwoing virtual parameters
	command->calcHash(KEY_SESSIONID, sessionId);
	command->calcHash(KEY_SESSIONIDX, messageIdx);
	command->calcHash(KEY_USERID, userId);

	// Hash of password is only calculated for admin commands
	if (sessionMode > UM_USER)
	{
		command->calcHash(KEY_USERKEY, userKey);
	}

	// Increment message counter
	if (inSession)
	{
		messageIdx++;
	}

	bool result = inSession && command->verifyHash();

	if (!result)
		Serial.println("Verification failed");

	return result;
}

void WebSession::endSession()
{
	sessionId = 0;
}

unsigned long WebSession::newSession(WebCommand *command)
{	
	t_user_record *user;

	// read open analog pin for real random number
	randomSeed(analogRead(A7));

	if (configData.loadUser(command->cmd_userId, &user))
	{
		this->userId = user->userId;
		this->userKey = user->userKey;
		this->messageIdx = 1;
		this->sessionId = random(1, 0x7FFFFFFF);
		this->sessionMode = user->userMode;
Serial.print("New Session ID: ");Serial.println(this->sessionId);
		return this->sessionId;		
	}
	else
	{
		return 0;
	}
}

WebSession webSession;