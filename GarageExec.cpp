
#include "GarageExec.h"
#include "ConfigData.h"
#include "GarageMotor.h"


void GarageExec::begin(WebPages * webPages)
{
	_webPages = webPages;
}

void GarageExec::execCommand(WebCommand *command)
{
	if (command->equals(CMD_GETSTATE))
	{
		#if WEBDUINO_SERIAL_DEBUGGING > 1
				Serial.println(F("*** Garage state ***"));
		#endif
		_webPages->StatusMessage();
	}
	else if (command->equals(CMD_OPEN))
	{

		#if WEBDUINO_SERIAL_DEBUGGING > 1
				Serial.println(F("*** Opening ***"));
		#endif
		garageMotor.Open();
		_webPages->ResultMessage(STATE_OK);
	}
	else if (command->equals(CMD_CLOSE))
	{
		#if WEBDUINO_SERIAL_DEBUGGING > 1
				Serial.println(F("*** Closing ***"));
		#endif
		garageMotor.Close();
		_webPages->ResultMessage(STATE_OK);
	}
	else
	{
		// command not found - return error message
		#if WEBDUINO_SERIAL_DEBUGGING > 1
		Serial.println(F("*** Undefined ***"));
		#endif
		_webPages->ErrorMessage(STATE_UNDEFINED, "Command not found");
	}

}

void GarageExec::setLevelRequired(WebCommand *cmd)
{
	// no command avaiable yet to require admin rights
	cmd->userMode = UM_USER;
}



GarageExec garageExec;