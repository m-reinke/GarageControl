#ifndef SensorBase_h
#define SensorBase_h

#include "Arduino.h"

/// <summary>
/// Posible states o fthe garage door
/// </summary>
enum DoorState {
	/// <summary>
	/// The door state is unknown - not yet determined
	/// </summary>
	DOOR_VOID = 0,
	/// <summary>
	/// The door is open
	/// </summary>
	DOOR_OPEN,
	/// <summary>
	/// The door state is unknown - neither open, nor closed - rather in between
	/// </summary>
	DOOR_UNKNOWN,
	/// <summary>
	/// The door is closed
	/// </summary>
	DOOR_CLOSED
};

// Key constants to represent the above door states
const String DOOR_VOID_KEY    = "void";
const String DOOR_OPEN_KEY    = "open";
const String DOOR_UNKNOWN_KEY = "unknown";
const String DOOR_CLOSED_KEY  = "closed";

/// <summary>
/// Abstract interface for the garage state sensors
/// </summary>
class SensorBase
{
public:
	/// <summary>
	/// Gets the door state as readable text.
	/// </summary>
	/// <returns>the door state</returns>
	virtual String GetDoorStateText() = 0;

	/// <summary>
	/// Gets the door state as text key value.
	/// </summary>
	/// <returns>the door state</returns>
	virtual String GetDoorStateKey() = 0;

	/// <summary>
	/// Gets the numeric state of the door. <seealso>DoorState</seealso>
	/// </summary>
	/// <returns></returns>
	virtual int GetDoorState() = 0;
};

#endif