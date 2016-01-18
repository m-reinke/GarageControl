// HashTest.h

#ifndef _HASHTEST_h
#define _HASHTEST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SipHash_2_4.h>
#include "ConfigData.h"

class HashTestClass
{
 protected:


 public:
	void init();
	void printHash(String data);
	void calcHash(String value);
};

extern HashTestClass HashTest;

#endif

