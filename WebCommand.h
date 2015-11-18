// WebCommand.h

#ifndef _WEBCOMMAND_h
#define _WEBCOMMAND_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

typedef struct
{
	String key, value;
} KeyValue;


class WebCommand
{
public:
	String command;
	uint16_t cmd_userId;
	uint64_t cmd_hash;
	uint16_t userMode;
public:
	void startNewCommand();
	void addParam(String key, String value);
	void calcHash(String name, String value);
	void calcHash(String name, uint32_t value);
	bool equals(String command);

	bool verifyHash();

	int getIntValue(String key);
	String getValue(String key);

private:
	KeyValue keyValue[5];
	int keyValueIdx;

	void calcHash(String value);
};


#endif

