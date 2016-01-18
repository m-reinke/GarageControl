/* 
* LedControl.cpp
*
* Created: 09.03.2014 09:11:56
* Author: Martin
*/


#include "LedControl.h"

// default constructor
LedControl::LedControl()
{
  _blinkIsOn = false;

  pinMode(OutRed,   OUTPUT);
  pinMode(OutGreen, OUTPUT);
  pinMode(OutBlue,  OUTPUT);
  

} //LedControl

void LedControl::SetLed(unsigned int color, unsigned int blinkStatus)
{
	// return, if nothing changed
	if (color == Color && blinkStatus == BlinkStatus)	
		return;
	
	// Set variables
	Color = color;
	BlinkStatus = blinkStatus;
		
	// set duration variables - or directly switch LED pins for static blink states
   switch (BlinkStatus) {
     case ALWAYS_ON:
	   SetLedLines (color);
	   return;
	   
	 case ALWAYS_OFF:
		AllOff();  
		return;
		
	case BLINK:
		_DarkDuration  = BlinkDarkDuration;
		_LightDuration = BlinkLightDuration;
		break;
		
	case FLASH:
		_DarkDuration  = FlashDarkDuration;
		_LightDuration = FlashLightDuration;
		break;
   }
		
   // set LED pins and remember state
   SetLedLines (color);
	_lastChange = millis() - _LightDuration;
	_blinkIsOn = true;
}


void LedControl::AllOff()
{
    digitalWrite (OutRed,   LED_OFF);
    digitalWrite (OutGreen, LED_OFF);
    digitalWrite (OutBlue,  LED_OFF);	
}

unsigned int LedControl::InvertColor (unsigned int color)
{
	return (color ^ ColorWhite);
}

void LedControl::SetLedLine (unsigned int color, unsigned int lineColor, unsigned int line)
{
   //calculate line state
   unsigned int lineState = ((color & lineColor) > 0) ? LED_ON : LED_OFF;
   digitalWrite (line, lineState);
}

void LedControl::SetLedLines (unsigned int color)
{
  // set each of the RGB lines 
  SetLedLine (color, ColorRed  , OutRed  );
  SetLedLine (color, ColorGreen, OutGreen);	
  SetLedLine (color, ColorBlue , OutBlue );
}

void LedControl::execute()
{
   // only do something, if currently blinking
   if (BlinkStatus >= BLINK)
     Blink();
}

void LedControl::Blink()
{
	unsigned int now = millis();
	
	// check, if time to switch has passed
	unsigned int duration;	
	if (now > _lastChange)
	{
		duration = now - _lastChange;
	}
	else
	{
		duration = (now + _LightDuration + _DarkDuration) - (_lastChange + _LightDuration + _DarkDuration);
	}
	
	bool stateResult = (duration > _DarkDuration) && (duration < _DarkDuration + _LightDuration);
	
	// Test if current state remains
	if (stateResult == _blinkIsOn)
	  return;
	  
	_blinkIsOn = stateResult;  
	
	if (_blinkIsOn)
	{
      SetLedLines (Color);
	}
	else
	{
	  _lastChange = now;
      AllOff();
	}

}