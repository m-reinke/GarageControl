
#include "Arduino.h"
#include "GarageSensor.h"
#include "LedBase.h"

GarageSensor::GarageSensor() 
{
  _doorState = DOOR_VOID; 
  _doorStateSum = 0; 
  _doorStateCount = 0;
  _doorStatePtr = 0;
 
  _doorStateColors[DOOR_VOID]    = ColorRed;
  _doorStateColors[DOOR_OPEN]    = ColorRed;
  _doorStateColors[DOOR_UNKNOWN] = ColorBlue;
  _doorStateColors[DOOR_CLOSED]  = ColorGreen;
  _doorStateBlink[DOOR_VOID]     = FLASH;
  _doorStateBlink[DOOR_OPEN]     = BLINK;
  _doorStateBlink[DOOR_UNKNOWN]  = BLINK;
  _doorStateBlink[DOOR_CLOSED]   = FLASH;
}

void GarageSensor::begin(LedBase *led)
{
	_led = led;
 
	pinMode(InOpen, INPUT);
	pinMode(InClosed, INPUT);
}

int GarageSensor::GetDoorState ()
{
  return _doorState; 
}

String GarageSensor::GetDoorStateText ()
{
  switch (_doorState){
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

String GarageSensor::GetDoorStateKey ()
{
	switch (_doorState){
		case DOOR_OPEN:
		return "open";
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


void GarageSensor::loadDoorState ()
{
  int actualState;
  if (digitalRead(InOpen) == LOW)
    actualState = DOOR_OPEN;
  else if (digitalRead(InClosed) == LOW)
    actualState = DOOR_CLOSED;
  else
    actualState = DOOR_UNKNOWN;
    
  
  // Alten Status merken
  int oldState = _doorStates[_doorStatePtr];
  	
  // Neuen Status in array eintragen	
  _doorStates[_doorStatePtr] = actualState;
  _doorStateSum += actualState;

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

  if (_doorStateCount > 0)
  {
    doorStateResult = int (float(_doorStateSum) / _doorStateCount + 0.5);
  }
  
  if (doorStateResult == _doorState)	
    return;

  _doorState = doorStateResult;
  
  _led->SetLed(_doorStateColors[_doorState], _doorStateBlink[_doorState]);
}

void GarageSensor::execute()
{
  // Code der immer laufen soll kommt hier hin.
  loadDoorState ();
}

GarageSensor garageSensor;

