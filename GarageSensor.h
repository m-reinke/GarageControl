/*
  GarageSensor.h - Library for a Garage opener.
 */
#ifndef GarageSensor_h
#define GarageSensor_h

#include "Arduino.h"
#include "LedBase.h"
#include "SensorBase.h"


const int CountSumMax = 16;  

/// <summary>
/// Representing the two open/closed end switches of the garage door
/// Contains some declicking and denoising
/// Controling the RGB LED to display the door
/// </summary>
/// <seealso cref="SensorBase" />
class GarageSensor : public SensorBase
{
public:
  GarageSensor();

  /// <summary>
  /// Initialization, place in the setup() function of the ino file.
  /// </summary>
  /// <param name="garageSensor">The LED controll object.</param>
  void begin(LedBase *led);

  /// <summary>
  /// Executes this instance, place in the loop() function of the ino file.
  /// </summary>
  void execute();

  /// <summary>
  /// Gets the door state text.
  /// </summary>
  /// <returns>The door state as readable - html formated text</returns>
  String GetDoorStateText();

  /// <summary>
  /// Gets the door state key.
  /// </summary>
  /// <returns>The key string representing the door state</returns>
  String GetDoorStateKey();


  /// <summary>
  /// Gets the state of the door.
  /// </summary>
  /// <returns>An integer representation of the door state</returns>
  int GetDoorState();

private:
	/// <summary>
	/// Loads the state of the door by calculating a sliding average of states
	/// </summary>
	void loadDoorState();

	/// <summary>
	/// The current door state
	/// </summary>
	int _doorState;

	/// <summary>
	/// The intermediate door state sum 
	/// </summary>
	int _doorStateSum;

	/// <summary>
	/// The number of state measurements in the state sum
	/// </summary>
	int _doorStateCount;

	/// <summary>
	/// The pointer to for the current door state in the _doorStates array
	/// </summary>
	int _doorStatePtr;

	/// <summary>
	/// The door states array for the sliding average
	/// </summary>
	int _doorStates[CountSumMax];

	/// <summary>
	/// The threshold for door state open
	/// </summary>
	int _thresholdOpen;

	/// <summary>
	/// The threshold for door state closed
	/// </summary>
	int _thresholdClosed;

  LedBase *_led;
  
  /// <summary>
  /// The LED colors for each door state
  /// </summary>
  unsigned int _doorStateColors[4];

  /// <summary>
  /// The blink rate for each door state
  /// </summary>
  unsigned int _doorStateBlink[4];
};

extern GarageSensor garageSensor;

#endif

