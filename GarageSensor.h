/*
  GarageSensor.h - Library for a Garage opener.
 */
#ifndef GarageSensor_h
#define GarageSensor_h

#include "Arduino.h"
#include "LedBase.h"
#include "SensorBase.h"


const int CountSumMax = 16;  

class GarageSensor : public SensorBase
{
public:
  GarageSensor();
  void begin(LedBase *led);
  void execute();
  String GetDoorStateText ();    
  String GetDoorStateKey ();
  int GetDoorState ();    
//  void allOff();
private:
  void displayDoorState ();
  void displayOpenState ();
  void evaluateDoorState ();
  void loadDoorState ();
  void setDoorStateValue(int value);

  int _doorState;
  int _doorStateSum;
  int _doorStateCount;
  int _doorStatePtr;
  int _doorStates [CountSumMax]; 
  int _thresholdOpen;
  int _thresholdClosed;
  LedBase *_led;
  
  unsigned int _doorStateColors[4];
  unsigned int _doorStateBlink[4];
};

extern GarageSensor garageSensor;

#endif

