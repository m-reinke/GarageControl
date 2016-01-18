// WebCommand.h

#ifndef _WEBCOMMAND_h
#define _WEBCOMMAND_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SipHash_2_4.h"
#include "ConfigData.h"

/// <summary>
/// The maximum lenght of a command keyword
/// </summary>
const int COMMAND_SIZE = 16;

/// <summary>
/// The maximum lenght of a command param keyword
/// </summary>
const int PARAM_KEY_SIZE = 10;

/// <summary>
/// The maximum lenght of a command parameter value
/// </summary>
const int PARAM_VALUE_SIZE = 40;

/// <summary>
/// The maximum number of parameters for a command
/// </summary>
const uint8_t KEY_VALUE_LENGTH = 5;

/// <summary>
/// A command parameter
/// </summary>
typedef struct
{
	char key[PARAM_KEY_SIZE];
	char value[PARAM_VALUE_SIZE];
} KeyValue;

/// <summary>
/// Represents a command from a remote client
/// The command has one command keyword and a number of named parameters
/// </summary>
class WebCommand
{
public:
	/// <summary>
	/// The command keyword
	/// </summary>
	char command[COMMAND_SIZE];

	/// <summary>
	/// The user/device identifier who placed the command
	/// </summary>
	uint16_t cmd_userId;
	
	/// <summary>
	/// The command hash value, calcualted by the remote device
	/// </summary>
	uint64_t cmd_hash;

	/// <summary>
	/// The user mode of the caller
	/// </summary>
	uint16_t userMode;
public:
	/// <summary>
	/// Starts a new command and resets hash calculation.
	/// </summary>
	/// <param name="cmd">The command string.</param>
	void startNewCommand(char* cmd);

	/// <summary>
	/// Adds a parameter to the command.
	/// </summary>
	/// <param name="key">Parameter key.</param>
	/// <param name="value">Parameter value.</param>
	/// <returns></returns>
	bool addParam(char* key, char* value);

	/// <summary>
	/// Calculates the hash of the key value pair.
	/// </summary>
	/// <param name="name">The key name.</param>
	/// <param name="value">The value.</param>
	void calcHash(char* name, char* value);

	/// <summary>
	/// Calculates the hash of the key value pair.
	/// </summary>
	/// <param name="name">The key name.</param>
	/// <param name="value">The int value.</param>
	void calcHash(char* name, uint32_t value);

	/// <summary>
	/// Checks if the command has this command verb.
	/// </summary>
	/// <param name="command">The command verb.</param>
	/// <returns>true if this is the command</returns>
	bool equals(char* command);

	/// <summary>
	/// Calculates the SIP hash and verifies if the value is the same as the external value.
	/// </summary>
	/// <returns></returns>
	bool verifyHash();

	/// <summary>
	/// Gets the int value of a command param.
	/// </summary>
	/// <param name="key">The param key.</param>
	/// <returns>the param value or 0</returns>
	int getIntValue(char* key);

	/// <summary>
	/// Gets the string value of a given command param.
	/// </summary>
	/// <param name="key">The param key.</param>
	/// <returns>the param value or empty string</returns>
	char* getValue(char* key);

private:
	/// <summary>
	/// The command parameters
	/// </summary>
	KeyValue keyValue[KEY_VALUE_LENGTH];
	/// <summary>
	/// An empty string
	/// </summary>
	char EMPTY_STR = 0;

	/// <summary>
	/// The current number of parameters
	/// </summary>
	int keyValueIdx;

	/// <summary>
	/// Calculates the (partial) hash of a value.
	/// </summary>
	/// <param name="value">The value.</param>
	void calcHash(char* value);

	/// <summary>
	/// Copies a string
	/// </summary>
	/// <param name="dest">The destination string</param>
	/// <param name="source">The source string</param>
	/// <param name="maxLength">The maximum length of the string.</param>
	void copyTo(char* dest, char* source, uint16_t maxLength);
};


#endif

