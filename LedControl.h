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


/// <summary>
/// Controll class for a RGB LED
/// </summary>
/// <seealso cref="LedBase" />
class LedControl : public LedBase
{
//variables
public:
	/// <summary>
	/// The current color
	/// </summary>
	unsigned int Color;
	/// <summary>
	/// The current blink status
	/// </summary>
	unsigned int BlinkStatus;
private:
	/// <summary>
	/// Time in ticks of the last change
	/// </summary>
	unsigned long _lastChange;

	/// <summary>
	/// LED is in ON period during blinking
	/// </summary>
	bool _blinkIsOn;
	/// <summary>
	/// The currend dark duration while blinking
	/// </summary>
	unsigned int _DarkDuration;
	/// <summary>
	/// The current light duration while blinking
	/// </summary>
	unsigned int _LightDuration;

//functions
public:
  LedControl();
  /// <summary>
  /// Executes this instance. Has to be placed in the loop function of the ino file.
  /// Responsible for blinking of the LED
  /// </summary>
  void execute();

  /// <summary>
  /// Sets the led state.
  /// Blinking states are only available, when the Arduino is running in the loop function of the ino-File
  /// </summary>
  /// <param name="color">The color.</param>
  /// <param name="BlinkStatus">The blink state.</param>
  virtual void SetLed(unsigned int color, unsigned int BlinkStatus);

  /// <summary>
  /// Switch the LED off.
  /// </summary>
  virtual void AllOff();
private:
	/// <summary>
	/// Inverts the color.
	/// </summary>
	/// <param name="color">The color.</param>
	/// <returns>The inverse color</returns>
	unsigned int InvertColor(unsigned int color);

	/// <summary>
	/// Blinks the LED, if the time has come
	/// </summary>
	void Blink();

	/// <summary>
	/// Sets the output pins to get the required color.
	/// </summary>
	/// <param name="color">The color.</param>
	void SetLedLines(unsigned int color);

	/// <summary>
	/// Sets one output pin - only put to ON, if it is part of the required color
	/// </summary>
	/// <param name="color">The required color.</param>
	/// <param name="lineColor">The LED color of this output pin.</param>
	/// <param name="line">The output pin.</param>
	void SetLedLine(unsigned int color, unsigned int lineColor, unsigned int line);
}; //LedControl

#endif //__LEDCONTROL_H__
