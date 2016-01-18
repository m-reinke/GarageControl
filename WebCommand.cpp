// 
// 
// 

#include "WebCommand.h"

/// <summary>
/// Prints the hash as two 8 char ex values
/// </summary>
/// <param name="hash">The hash value.</param>
void printHash(uint64_t hash)
{
	uint32_t lo, hi;
	lo = hash & 0xFFFFFFFF;
	hi = (hash >> 32) & 0xFFFFFFFF;
	Serial.print(hi, HEX);Serial.print(".");Serial.print(lo, HEX);
}


/// <summary>
/// converts a 8-char (part of a) hex string to a 32 bit integer value.
/// </summary>
/// <param name="value">The hex string.</param>
/// <param name="offset">The offset in the string to start.</param>
/// <returns></returns>
uint32_t HexToUInt32(char *value, uint8_t offset = 0)
{
    int len = strlen(value);
	if (len - offset < 8)
		return 0;

	char hex[10];
	memcpy(hex, &value[offset], 8);
	hex[8] = 0;

	uint32_t result = strtoul(hex, 0, 16);

	Serial.print("HEX32: "); Serial.print(hex); Serial.print("->");Serial.println(result, HEX);

	return result;
}

/// <summary>
/// Converts a 16-char hex (hash) string to int64.
/// </summary>
/// <param name="value">The value.</param>
/// <returns>the int representation</returns>
uint64_t HexToUInt64(char *value)
{
	//Only strings with exactly 16 chars will be considered
	int len = strlen(value);
	if (len != 16)
	{
		Serial.print("Ilegal hash lenght: "); Serial.print(len);Serial.print(": ");Serial.println(value);
		return 0;
	}
		  
	uint32_t lo = HexToUInt32(value, 8);
	uint32_t hi = HexToUInt32(value);

	uint64_t result = (lo + ((uint64_t)hi << 32));
	if (result == 0)
	{
		// Assertion - should not happen
		Serial.print("Zero hash: "); Serial.print(value);Serial.print("->");Serial.print(hi, HEX);Serial.print(".");Serial.println(lo, HEX);
	}


	return result;
}

void WebCommand::calcHash(char *value)
{
	// just pass each char to the hashing
	int len = strlen(value);
	for (int i = 0; i < len; i++)
	{
		sipHash.updateHash(value[i]);
	}
}

void WebCommand::copyTo(char* dest, char* source, uint16_t maxLength)
{
	int len = strlen(source);
	if (len + 1 > maxLength)
	{
		Serial.print("Value too long ");Serial.print(source); Serial.print(" "); Serial.print(len);Serial.print(">");Serial.println(maxLength);
		while (1);
	}
	strcpy(dest, source);
}

void WebCommand::calcHash(char *name, char *value)
{
	Serial.print("Hashing ");Serial.print(name);Serial.print(" ");Serial.println(value);

	// pass parameter name and param value individually to the hashing
	calcHash(name);
	calcHash(value);
}

void WebCommand::calcHash(char *name, uint32_t value)
{
	//convert to string before hashing a numeric value
	char sValue[13];

	ultoa(value, sValue, 10);

	calcHash(name, sValue);
}

void WebCommand::startNewCommand(char* cmd)
{
	// register command keyword
	copyTo(this->command, cmd, COMMAND_SIZE);

	this->keyValueIdx = 0;
	memset(keyValue, 0, sizeof(KeyValue) * KEY_VALUE_LENGTH);

	// reset hashing
	sipHash.initFromRAM(configData.sipKey);

	// calc hash of command keyword
	calcHash(KEY_CMD, cmd);
}

bool WebCommand::addParam(char *key, char *value)
{
	Serial.print("adding param "); Serial.print(key); Serial.print(": ");Serial.println(value);

	if (strcmp(key, KEY_HASH) == 0)
	{
		// if parameter is external hash, then only remember this value - but do not include in hash calculation
		cmd_hash = HexToUInt64(value);
		Serial.print("Ext hash "); printHash(cmd_hash);Serial.println(" "); 
//		Serial.print("Ext hash "); printHash(cmd_hash);Serial.print(" in ");Serial.println(value);
	}
	else
	{
		// add all other parameters to hash calculation
		calcHash(key, value);
	}

	// User Id is only transferred in the connect command
	// Don't store as parameter
    if (strcmp(key, KEY_USERID) == 0)
	{
		cmd_userId = strtoul(value, 0, 10);

		Serial.print("User ID found "); Serial.println(cmd_userId);
	}
	else if (keyValueIdx < KEY_VALUE_LENGTH)
	{
		// every other paramter is put into the parameter list -
		copyTo(keyValue[keyValueIdx].key, key, PARAM_KEY_SIZE);
		copyTo(keyValue[keyValueIdx].value, value, PARAM_VALUE_SIZE);
		keyValueIdx++;
	}
	else 
	{
		// Parameter overflow!
		return false;
	}

	return true;
}

bool WebCommand::equals(char *command)
{
	//Serial.print("testing command "); Serial.println(command);
	return (strcmp(this->command, command) == 0);
}

bool WebCommand::verifyHash()
{
	sipHash.finish(); // result in BigEndian format

	uint64_t calculatedHash;
	memmove(&calculatedHash, sipHash.result, sizeof(uint64_t));

	Serial.print("Hash calc ");printHash(calculatedHash); Serial.print(" ext "); printHash(cmd_hash);Serial.println(" ");

	return (cmd_hash == calculatedHash);
}

int WebCommand::getIntValue(char *key)
{
	// Get string value and convert string to int
	char *value = getValue(key);
	return strtoul(value, 0, 10);
}

char *WebCommand::getValue(char *key)
{
	// iterate throug hash values to find the required parameter
	for (int i = 0; i < this->keyValueIdx; i++)
	{
		if (strcmp(key, keyValue[i].key) == 0)
		{
			return keyValue[i].value;
		}
	}

	Serial.print("Key not found "); Serial.println(key); 
	return &EMPTY_STR;
}

