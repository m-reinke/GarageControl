/* 
* LedControl.h
*
* Created: 09.03.2014 09:11:56
* Author: Martin
*/


#ifndef __LEDCONTROL_H__
#define __LEDCONTROL_H__

#include "Constants.h"
#include "LedBase.h"


class LedControl : public LedBase
{
//variables
public:
  unsigned int Color;
  unsigned int BlinkStatus;
private:
  unsigned long _lastChange;  
  bool _blinkIsOn;
  unsigned int _DarkDuration;
  unsigned int _LightDuration;

//functions
public:
  LedControl();
  void execute();
  virtual void SetLed(unsigned int color, unsigned int BlinkStatus);
  virtual void AllOff();
private:
  unsigned int InvertColor(unsigned int color);
  void Blink();
  void SetLedLines(unsigned int color);
  void SetLedLine (unsigned int color, unsigned int lineColor, unsigned int line);
}; //LedControl

#endif //__LEDCONTROL_H__
