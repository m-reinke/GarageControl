/*
  GarageMotor.h - Library for a Garage opener.
*/
#ifndef GarageMotor_h
#define GarageMotor_h

#include "Arduino.h"
#include <SPI.h>
#include "SensorBase.h"
#include "MotorBase.h"


/// <summary>
/// Output pin for the relais 
/// </summary>
const int RelaisOut = 6;


/// <summary>
/// The ms hold duration for a relais pulse
/// </summary>
const unsigned int HOLD_DURATION = 500;


/// <summary>
/// The ms lock duration after a relais pulse
/// </summary>
const unsigned int LOCK_DURATION = 3000;

/// <summary>
/// Not used yet
/// </summary>
//const unsigned int REACTION_TIME = 3000;

/// <summary>
/// Relais trigger class
/// Checks for open or close, whether the garage is already at that state.
/// Then triggers the relais for a certain time. After that the Relais is locked for some duration
/// In order to get the garage door in motion before the next command
/// </summary>
/// <seealso cref="MotorBase" />
class GarageMotor : public MotorBase
{
  public:
	  /// <summary>
	  /// Initializes a new instance of the <see cref="GarageMotor"/> class.
	  /// </summary>
	  GarageMotor();

	  /// <summary>
	  /// Initialization, place in the setup() function of the ino file.
	  /// </summary>
	  /// <param name="garageSensor">The garage sensor.</param>
	  void begin(SensorBase *garageSensor);

	  /// <summary>
	  /// Executes this instance, place in the loop() function of the ino file.
	  /// </summary>
	  void execute();

	  /// <summary>
	  /// Schedules the open sequence for the garage door.
	  /// </summary>
	  void Open();

	  /// <summary>
	  /// Schedules the closing sequence for the garage door.
	  /// </summary>
	  void Close();

	  /// <summary>
	  /// Determines whether this instance can trigger a new open/close command.
	  /// </summary>
	  /// <returns>true if a trigger is allowed</returns>
	  bool canTrigger();
  private:
    SensorBase *_garageSensor; 

	/// <summary>
	/// Sets the relais to connected.
	/// </summary>
	void triggerRelais();

	/// <summary>
	/// Sets the relais to open.
	/// </summary>
	void untriggerRelais();


	/// <summary>
	/// Determines whether the delay has passed to untrigger.
	/// </summary>
	/// <returns>true if delay has passed</returns>
	bool canUntrigger();


	/// <summary>
	/// Checks whether the delay has passed.
	/// </summary>
	/// <param name="triggerTime">The triggerstart time time.</param>
	/// <param name="timeSpan">The time span.</param>
	/// <returns>true if trigger is passed</returns>
	bool delayPassed(unsigned int triggerTime, unsigned int timeSpan);

	/// <summary>
	/// Checks whether the relais is triggered at the moment
	/// </summary>
	bool isTriggered;

	/// <summary>
	/// Checks whether the relais is locked after the last tri
	/// </summary>
	bool isLocked;


	/// <summary>
	/// Tick time of the trigger start
	/// </summary>
	unsigned int closeTime;

	/// <summary>
	/// Tick time when the lock period ends
	/// </summary>
	unsigned int lockEndTime;
	
	
};

extern GarageMotor garageMotor;

#endif


