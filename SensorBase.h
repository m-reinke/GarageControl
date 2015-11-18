#ifndef SensorBase_h
#define SensorBase_h

#include "Arduino.h"

enum DoorState {
	DOOR_VOID = 0,
	DOOR_OPEN,
	DOOR_UNKNOWN,
	DOOR_CLOSED
};


class SensorBase
{
public:
	virtual String GetDoorStateText() = 0;
	virtual String GetDoorStateKey() = 0;
	virtual int GetDoorState() = 0;
};

#endif