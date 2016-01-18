#pragma once

#ifndef __LEDBASE_H__
#define __LEDBASE_H__

#include "Constants.h"
#include "LedBase.h"

// pure colors - only one partial LED
const unsigned int ColorRed = 0x1;
const unsigned int ColorGreen = 0x2;
const unsigned int ColorBlue = 0x4;

// mixed colors - two partial LED's
const unsigned int ColorYellow = 0x3;
const unsigned int ColorPurple = 0x5;
const unsigned int ColorCyan   = 0x6;

// mixed color - all LED's
const unsigned int ColorWhite = 0x7;

// No LED
const unsigned int ColorBlack = 0x0;


/// <summary>
/// Posible LED states
/// </summary>
enum BlinkStates {
	// On, no blinking
	ALWAYS_ON = 0,
	// Off, no blinking
	ALWAYS_OFF,
	// Regular blinking
	BLINK,
	// Only short flashes
	FLASH
};

// Times for flash blinking
const unsigned int FlashDarkDuration = 2500;
const unsigned int FlashLightDuration = 70;

// Times for regular blinking
const unsigned int BlinkDarkDuration = 250;
const unsigned int BlinkLightDuration = 250;



/// <summary>
/// Abstract interface for the LedControl class for a RGB LED
/// </summary>
class LedBase
{
public:
	/// <summary>
	/// Sets the led state.
	/// </summary>
	/// <param name="color">The color.</param>
	/// <param name="BlinkStatus">The blink status.</param>
	virtual void SetLed(unsigned int color, unsigned int BlinkStatus) = 0;

	/// <summary>
	/// All partial LED's off.
	/// </summary>
	virtual void AllOff() = 0;
};

#endif