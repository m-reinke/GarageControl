// 
// 
// 

#include "WebCommand.h"
#include "SipHash_2_4.h"
#include "ConfigData.h"

void WebCommand::calcHash(String value)
{
	for (int i = 0; i < value.length(); i++)
	{
		sipHash.updateHash(value.charAt(i));
	}
}

void WebCommand::calcHash(String name, String value)
{
	calcHash(name);
	calcHash(value);
}

void WebCommand::calcHash(String name, uint32_t value)
{
	calcHash(name, String(value));
}

void WebCommand::startNewCommand()
{
	sipHash.initFromRAM(configData.sipKey);
}

void WebCommand::addParam(String key, String value)
{
	if (key.equals(KEY_HASH))
	{
		cmd_hash = value.toInt();
	}
	else
	{
		calcHash(key, value);
	}


	if (key.equals(KEY_CMD))
	{
		command = value;
	}
	else if (key.equals(KEY_USERID))
	{
		cmd_userId = value.toInt();
	}
	else if (keyValueIdx < 5)
	{
		keyValue[keyValueIdx].key = key;
		keyValue[keyValueIdx].value = value;
		keyValueIdx++;
	}

}

bool WebCommand::equals(String command)
{
	return this->command.equals(command);
}



bool WebCommand::verifyHash()
{
	sipHash.finish(); // result in BigEndian format

	uint64_t calculatedHash = (uint64_t)(*sipHash.result);

	return (cmd_hash == calculatedHash);
}

int WebCommand::getIntValue(String key)
{
	String value = getValue(key);
	return value.toInt();
}

String WebCommand::getValue(String key)
{
	for (int i = 0; i < this->keyValueIdx; i++)
	{
		if (key.equals(keyValue[i].key))
		{
			return keyValue[i].value;
		}
	}
	return String();
}

