// 
// 
// 

#include "HashTest.h"

void HashTestClass::init()
{


}

void printHsh(uint64_t hash)
{
	uint32_t lo, hi;
	lo = hash & 0xFFFFFFFF;
	hi = (hash >> 32) & 0xFFFFFFFF;
	Serial.print(hi, HEX);Serial.print(".");Serial.print(lo, HEX);
}


void HashTestClass::printHash(String data)
{

	configData.sipKey;

	sipHash.initFromRAM(configData.sipKey);

	calcHash(data);

	sipHash.finish(); // result in BigEndian format

	uint64_t calculatedHash;
	memmove(&calculatedHash, sipHash.result, sizeof(uint64_t));

	Serial.print("Hash calc "); printHsh(calculatedHash); Serial.print(" ext "); Serial.print(data);Serial.println(" ");
}

void HashTestClass::calcHash(String value)
{
	for (int i = 0; i < value.length(); i++)
	{
		sipHash.updateHash(value.charAt(i));
	}
}


HashTestClass HashTest;

