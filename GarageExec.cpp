// 
// 
// 

#include "GarageExec.h"
#include "ConfigData.h"
#include "GarageMotor.h"


void GarageExec::begin(WebPages * webPages)
{
	_webPages = webPages;
}

void GarageExec::execCommand(WebCommand command)
{
	if (command.equals(CMD_GETSTATE))
	{
		#if WEBDUINO_SERIAL_DEBUGGING > 1
				Serial.println(F("*** Status ***"));
		#endif
		_webPages->StatusMessage();
	}
	else if (command.equals(CMD_OPEN))
	{
		#if WEBDUINO_SERIAL_DEBUGGING > 1
				Serial.println(F("*** Opening ***"));
		#endif
		garageMotor.Open();
		_webPages->ResultMessage(STATE_OK);
	}
	else if (command.equals(CMD_CLOSE))
	{
		#if WEBDUINO_SERIAL_DEBUGGING > 1
				Serial.println(F("*** Closing ***"));
		#endif
		garageMotor.Close();
		_webPages->ResultMessage(STATE_OK);
	}
	else if (command.equals(CMD_USER))
	{
		#if WEBDUINO_SERIAL_DEBUGGING > 1
				Serial.println(F("*** User ***"));
		#endif

		//configData.setUser()
		_webPages->ResultMessage(STATE_OK);
	}
	else
	{
		#if WEBDUINO_SERIAL_DEBUGGING > 1
		Serial.println(F("*** Undefined ***"));
		#endif
		_webPages->ResultMessage(STATE_UNDEFINED);
	}

}

void GarageExec::setLevelRequired(WebCommand cmd)
{
	if (cmd.equals(CMD_USER))
		cmd.userMode = UM_ADMIN;
	else 
		cmd.userMode = UM_USER;
}

GarageExec garageExec;