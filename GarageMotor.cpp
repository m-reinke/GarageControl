 /*
  Garage motor control
  For a garage opener with a simple two wire control button
  close the two wires for a certain time with the help of a relais 
 */

//#include <Servo.h>
#include "GarageMotor.h"

GarageMotor::GarageMotor()
{
  // Enable control of a servo on pin 9:

  ;
}

void GarageMotor::begin(SensorBase *garageSensor)
{
	// use the garage sensor to verify if the open- or close command can be called
	this->_garageSensor = garageSensor;
	// set the pin states
	pinMode(RelaisOut, OUTPUT);
	digitalWrite(RelaisOut, LOW);
}


void GarageMotor::execute()
{
	if (canUntrigger())
	{
		untriggerRelais();
	}
	else
	{
		// check for lock and unlock		
		canTrigger();		
	}
}

bool GarageMotor::delayPassed(unsigned int timestamp, unsigned int timeSpan)
{
	unsigned int now = millis();
	bool passed = 
		(timestamp == 0) ||
		((timestamp <  timeSpan) && (now < timeSpan) && (now > timestamp)) ||
		// timer overrun
		((timestamp >= timeSpan) &&                     (now > timestamp));
	
	return passed;	
}

bool GarageMotor::canUntrigger()
{
	bool result = isTriggered && delayPassed(closeTime, HOLD_DURATION);
	
	return result;
}

bool GarageMotor::canTrigger()
{
	bool result = !isLocked || delayPassed(lockEndTime, LOCK_DURATION);

	if (isLocked && result)
		// reset locked state after last trigger
		isLocked = false;
		
	return result;
} 

void GarageMotor::triggerRelais()
{
	//Serial.println("Trigger relais");

	if (isTriggered || !canTrigger())
	  return;
	  
	isTriggered = true;
	closeTime = millis() + HOLD_DURATION;
	lockEndTime = closeTime + LOCK_DURATION;

	//Serial.print("Trigger relais ");Serial.print(RelaisOut);	Serial.print(" for ms: ");Serial.print(HOLD_DURATION);	Serial.print(" lock for ");Serial.println(LOCK_DURATION);

	digitalWrite(RelaisOut, HIGH);	
}

void GarageMotor::untriggerRelais()
{
	if (!isTriggered)
	  return;

	//Serial.println("Untrigger relais");

	isTriggered = false;

	// lock the relais to prevent too many consecutive triggers
	isLocked = true;
	lockEndTime = millis() + LOCK_DURATION;

	digitalWrite(RelaisOut, LOW);
}


void GarageMotor::Open()
{
  if (_garageSensor->GetDoorState() == DOOR_OPEN)
	return;
	
  triggerRelais();		
}

void GarageMotor::Close()
{
  // only 
  if (_garageSensor->GetDoorState() == DOOR_CLOSED)
    return;
  
  triggerRelais();
}

GarageMotor garageMotor;