 /*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
  
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
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
	this->_garageSensor = garageSensor;
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
		isLocked = false;
		
	return result;
} 

void GarageMotor::triggerRelais()
{
	if (isTriggered || !canTrigger())
	  return;
	  
	isTriggered = true;
	closeTime = millis() + HOLD_DURATION;
	lockEndTime = closeTime + LOCK_DURATION;
    digitalWrite(RelaisOut, HIGH);	
}

void GarageMotor::untriggerRelais()
{
	if (!isTriggered)
	  return;
	
	isTriggered = false;
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
  if (_garageSensor->GetDoorState() == DOOR_CLOSED)
    return;
  
  triggerRelais();
}

GarageMotor garageMotor;