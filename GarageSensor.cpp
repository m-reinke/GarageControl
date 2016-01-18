
#include "Arduino.h"
#include "GarageSensor.h"
#include "LedBase.h"

GarageSensor::GarageSensor()
{
	// initialize variables
	_doorState = DOOR_VOID;
	_doorStateSum = 0;
	_doorStateCount = 0;
	_doorStatePtr = 0;

	// initialize door state array values
	_doorStateColors[DOOR_VOID] = ColorRed;
	_doorStateColors[DOOR_OPEN] = ColorRed;
	_doorStateColors[DOOR_UNKNOWN] = ColorBlue;
	_doorStateColors[DOOR_CLOSED] = ColorGreen;
	_doorStateBlink[DOOR_VOID] = FLASH;
	_doorStateBlink[DOOR_OPEN] = BLINK;
	_doorStateBlink[DOOR_UNKNOWN] = BLINK;
	_doorStateBlink[DOOR_CLOSED] = FLASH;
}

void GarageSensor::begin(LedBase *led)
{
	_led = led;

	// set Arduino pins for the two sensors
	pinMode(InOpen, SENSOR_PORT_MODE);
	pinMode(InClosed, SENSOR_PORT_MODE);
}

int GarageSensor::GetDoorState()
{
	// just return the variable value
	// calculation of the real state in the execute function
	return _doorState;
}

String GarageSensor::GetDoorStateText()
{
	// take state variable and return text
	switch (_doorState) {
	case DOOR_OPEN:
		return "<font color=""red"">open</font>";
		break;
	case DOOR_CLOSED:
		return "closed";
		break;
	case DOOR_UNKNOWN:
		return "unknown";
		break;
	default:
		return "not initialized";
	}
}

String GarageSensor::GetDoorStateKey()
{
	// take state variable and return one key constant value
	switch (_doorState) {
	case DOOR_OPEN: return DOOR_OPEN_KEY;
	case DOOR_CLOSED:return DOOR_CLOSED_KEY;
	case DOOR_UNKNOWN:return DOOR_UNKNOWN_KEY;
	default:return DOOR_VOID_KEY;
	}
}


void GarageSensor::loadDoorState()
{
	// ToDo get better declicking and de-noising algorithm


	int actualState;
	if (digitalRead(InOpen) == LOW)
		actualState = DOOR_OPEN;
	else if (digitalRead(InClosed) == LOW)
		actualState = DOOR_CLOSED;
	else
		actualState = DOOR_UNKNOWN;


	// Remember old state
	int oldState = _doorStates[_doorStatePtr];

	// Place new state in array	
	_doorStates[_doorStatePtr] = actualState;
	_doorStateSum += actualState;

	// calculate moving average
	_doorStatePtr++;
	if (_doorStatePtr >= CountSumMax)
		_doorStatePtr = 0;


	if (_doorStateCount >= CountSumMax)
	{
		_doorStateSum -= oldState;
	}
	else
	{
		_doorStateCount++;
	}

	int doorStateResult = DOOR_VOID;

	// trigger in center
	if (_doorStateCount > 0)
	{
		// todo could put in hysteresis
		doorStateResult = int(float(_doorStateSum) / _doorStateCount + 0.5);
	}

	// Determine wether door state has changed
	if (doorStateResult == _doorState)
		return;

	_doorState = doorStateResult;
	_led->SetLed(_doorStateColors[_doorState], _doorStateBlink[_doorState]);

	Serial.print("door state changed to "); Serial.println(this->GetDoorStateKey());
	//  Serial.print("S: "); Serial.println(_doorState);
}

void GarageSensor::execute()
{
	// Just calculate the door state
	loadDoorState();
}

GarageSensor garageSensor;

