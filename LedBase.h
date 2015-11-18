#pragma once

#ifndef __LEDBASE_H__
#define __LEDBASE_H__

#include "Constants.h"
#include "LedBase.h"

const unsigned int ColorRed = 0x1;
const unsigned int ColorGreen = 0x2;
const unsigned int ColorBlue = 0x4;

const unsigned int ColorYellow = 0x3;
const unsigned int ColorPurple = 0x5;
const unsigned int ColorCyan = 0x6;

const unsigned int ColorWhite = 0x7;
const unsigned int ColorBlack = 0x0;

enum BlinkStates {
	ALWAYS_ON = 0,
	ALWAYS_OFF,
	BLINK,
	FLASH
};

const unsigned int FlashDarkDuration = 2500;
const unsigned int FlashLightDuration = 70;
const unsigned int BlinkDarkDuration = 250;
const unsigned int BlinkLightDuration = 250;



class LedBase
{
public:
	virtual void SetLed(unsigned int color, unsigned int BlinkStatus) = 0;
	virtual void AllOff() = 0;
};

#endif