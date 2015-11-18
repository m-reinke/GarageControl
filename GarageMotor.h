/*
  GarageMotor.h - Library for a Garage opener.
*/
#ifndef GarageMotor_h
#define GarageMotor_h

#include "Arduino.h"
#include <SPI.h>
#include "SensorBase.h"

const int RelaisOut = 6;
const unsigned int HOLD_DURATION = 500;
const unsigned int LOCK_DURATION = 3000;
const unsigned int REACTION_TIME = 3000;

class GarageMotor
{
  public:
    GarageMotor();
    void begin(SensorBase *garageSensor);
    void execute();
	void Open();
	void Close();
	bool canTrigger();
  private:
    SensorBase *_garageSensor; 
	void triggerRelais();
	void untriggerRelais();
	bool canUntrigger();
	bool delayPassed(unsigned int triggerTime, unsigned int timeSpan);
	bool isTriggered;
	bool isLocked;
	unsigned int closeTime;
	unsigned int lockEndTime;
	
	
};

extern GarageMotor garageMotor;

#endif


