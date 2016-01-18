/* 
* ConfigData.cpp
*
* Created: 16.08.2014 15:24:01
* Author: Martin
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <SD.h>
#include "ConfigData.h"
#include "Constants.h"


// only used for debugging with CC3000 shield
#ifdef CC3000SHIELD
#include "IpSetupCc3000.h"
#endif

const char configFileName[] = "arduino.cfg";

void ConfigData::begin(LedBase *led)
{
	this->led = led;
	pinMode(PIN_SDCARD, OUTPUT);	
	
	// first try, if an SD card with a config file is available and valid
	bool success = this->loadConfigFromSdCard();
	if (success){
		baseConfigChanged = true;
		Serial.println("Loaded from SdCard");		
		this->printConfig();
		return;
	}
	
	// otherwise try config in EEPROM
	success = this->loadConfigFromEEPROM();
	if (success)
	{
	    Serial.println("Loaded from EEPROM");		
        baseConfigChanged = false;
		this->printConfig();
	}
	else
	{
		// found no config at all
		led->SetLed(ColorRed, ALWAYS_ON);
		while(1);
	}
}

bool ConfigData::isBaseConfigChanged()
{
	return baseConfigChanged;
}

bool ConfigData::setUser(uint8_t idx, uint16_t id, uint16_t key, uint16_t mode)
{
	// ilegal user index?
	if (idx >= EE_USER_COUNT)
	{
		return false;
	}

	// set user record in RAM
	user_records[idx].userId = id;
	user_records[idx].userKey = key;
	user_records[idx].userMode = mode;

	Serial.print("User changed ");
	printUser(idx);

	// Save user record to EEPROM
	saveUserRecord(idx);

	return true;
}

///<summary>offset of user record in EEPROM</summary>
///<param name='idx'>User index in array</param>
///<returns>Offset value of user</returns>
uint16_t userOffset(uint8_t idx)
{
	return EE_USER_OFFSET + idx * sizeof(t_user_record);
}

bool ConfigData::saveUserRecord(uint8_t idx)
{

	Serial.print("Saving user at "); Serial.print(idx); Serial.print(", offset ");

	// Pointer to first byte in RAM
	byte *p = (byte*)&user_records[idx];

	// index of first byte in EEPROM
	uint16_t e = userOffset(idx);
	Serial.println(e);

	// Fill bytes
	for (uint16_t i = 0; i < sizeof(t_user_record); i++)
	{
		EEPROM.write(e++, *p);
		p++;
	}

}


void printIpAddress (uint32_t ipAddress){

	Serial.print(ipAddress, HEX);
	Serial.print(" - ");

	for (int i = 0; i < 4; i++)
	{
		uint32_t ipx = ipAddress >> ((i)*8);
		Serial.print((ipx & 0xFF));
		ipx /= 256;
		if (i < 3)
			Serial.print(".");
		else
			Serial.println(" ");
	}	
}

void printMacAddress(uint8_t mac[]) {

	for (int i = 0; i < 6; i++)
	{
		if (mac[i] < 0x10)
			Serial.print("0");

		Serial.print(mac[i], HEX);
		if (i != 5)
			Serial.print(":");
		else
			Serial.println(" ");
	}
}

void printSipKey(uint8_t key[]) {

	for (int i = 0; i < 16; i++)
	{
		if (key[i] < 0x10)
			Serial.print("0");

		Serial.print(key[i], HEX);
		if (i == 15)
			Serial.println(" ");
	}
}



void ConfigData::printConfig(){

	Serial.println("v-----------Configuration------------v");

	Serial.print("version "); Serial.println(this->configVersion);

	Serial.print("ip address "); printIpAddress(this->ipAddress); 
	Serial.print("net mask "); printIpAddress(this->netmask);
	Serial.print("gateway "); printIpAddress(this->gateway);
	Serial.print("dns server "); printIpAddress(this->dns);

	Serial.print("MAC addr "); printMacAddress(this->macAddress);
	Serial.print("ssid "); Serial.println(this->ssid);
	Serial.print("wlanKey "); Serial.println(this->wlanKey);

	Serial.print("Sipkey "); printSipKey(this->sipKey);


	Serial.println("users:");
	for (uint16_t i = 0; i < EE_USER_COUNT; i++)
	{
		if (this->user_records[i].userMode > UM_INACTIVE){
			printUser(i);
		}
	}		
	Serial.println("^-----------Configuration------------^\n");	
}

void ConfigData::printUser(uint16_t i) {
	Serial.print(i);
	Serial.print(" id: "); Serial.print(this->user_records[i].userId, HEX);
	Serial.print(", key: "); Serial.print(this->user_records[i].userKey, HEX);
	Serial.print(", mode: "); Serial.println(this->user_records[i].userMode, HEX);
}

// Print comparison of two values to serial
void compareInt (uint16_t v1, uint16_t v2){
	Serial.print(v1);
	Serial.print(" - ");
	Serial.println(v2);
}

// default constructor
ConfigData::ConfigData()
{
	
} //ConfigData

// default destructor
ConfigData::~ConfigData()
{
} //~ConfigData

t_eeprom_config buffer;

bool ConfigData::loadConfigFromEEPROM()
{	
	// read EEPROM to buffer
	byte *p = (byte*)&buffer;
	uint16_t ee = 0;
	for (uint16_t i = 0; i < EE_CONFIG_LEN; i++)
	{
		*p = EEPROM.read(ee++);
		p++;
	}

	// check Buffer
	bool result = isBufferValid(&buffer);
	if (result)
	{
		// transfer to configData object
		loadFromBuffer(&buffer);
	}
	
	return result;
}

void ConfigData::loadFromBuffer(t_eeprom_config* buffer)
{
	this->configVersion = buffer->configVersion;
	this->useDHCP = buffer->useDHCP;
	this->ipAddress = buffer->ipAddress;
	this->gateway = buffer->gateway;
	this->netmask = buffer->netmask;
	this->dns = buffer->dns;
	memcpy(this->macAddress, buffer->macAddress, sizeof(buffer->macAddress));
	memcpy(this->ssid, buffer->ssid, sizeof(buffer->ssid));
	memcpy(this->wlanKey, buffer->wlanKey, sizeof(buffer->wlanKey));
	memcpy(this->sipKey, buffer->sipKey, sizeof(buffer->wlanKey));

	memcpy(this->user_records, buffer->users, sizeof(t_user_record) * EE_USER_COUNT);
}

bool ConfigData::isBufferValid(t_eeprom_config* buffer)
{
	
	uint16_t version    = buffer->configVersion;
	uint16_t startKey   = buffer->startKey;
	uint16_t endKey     = buffer->endKey;
	uint16_t userEndKey = buffer->userEndKey;
	
	bool result = (version >= EE_CONFIG_VERSION);
	result &= (startKey == EE_CONFIG_START_KEY);
	result &= (endKey == EE_CONFIG_END_KEY);
	result &= (userEndKey == EE_USER_END_KEY);
	
	// only valid, if the version number and the three key values are correct

	if (result)
	{
		Serial.println("Buffer valid");
	}
	else
	{
		Serial.println("Buffer invalid:");
		compareInt(version, EE_CONFIG_VERSION);
		compareInt(startKey, EE_CONFIG_START_KEY);
		compareInt(endKey, EE_CONFIG_END_KEY);
		compareInt(userEndKey, EE_USER_END_KEY);
	}

	return result;
}


bool ConfigData::loadConfigFromSdCard()
{ 
	// Activate SD Card
	SD.begin(PIN_SDCARD);
	
	t_eeprom_config buffer;
	t_user_record sdUser_records[EE_USER_COUNT];
	uint16_t userEndKey = 0;
	uint32_t fileSize = 0;

	byte *p_eeprom_config = (byte*)&buffer;	
	byte *p_user_config = (byte*)&sdUser_records;	
	byte *p_userEndKey = (byte*)&userEndKey;
	
	
	 // open the file for reading:
	 File cfgFile = SD.open(configFileName);
	 if (cfgFile) {
Serial.println("File opened");		 
		fileSize = cfgFile.size();
		 if (fileSize < EE_CONFIG_LEN)
		 {
Serial.print("Filesize too small "); 
			compareInt(fileSize, EE_CONFIG_LEN);
			 
			cfgFile.close();
			// Deactivate SD Card
			digitalWrite(PIN_SDCARD, HIGH);
			return false;
		 }
		 
		 cfgFile.readBytes(p_eeprom_config, EE_CONFIG_LEN);
		 
		 // close the file:
		 cfgFile.close();
	 } 
	 else 
	 {
        Serial.println("Config file not present");
		// Deactivate SD Card
		digitalWrite(PIN_SDCARD, HIGH);
		return false;
	 }

	bool result = isBufferValid(&buffer);

	if (result)
	{
      Serial.println("File config consistent");
      loadFromBuffer(&buffer);

	  // write data to eeprom
	  byte *p = (byte*)&buffer;
	  for (uint16_t i = 0; i < EE_CONFIG_LEN; i++)
	  {
		  EEPROM.write(i, *p);
		  p++;
	  }
	} 
		
	// Deactivate SD Card
	digitalWrite(PIN_SDCARD, HIGH);

	return result;
}


bool ConfigData::loadUser( uint16_t userId, t_user_record **userRecord)
{
	//Iterate to get user with given ID
	for (uint16_t i = 0; i < EE_USER_COUNT; i++)
	{
		if (this->user_records[i].userId == userId){
			
			*userRecord = &this->user_records[i];
			
			return (this->user_records[i].userMode != UM_INACTIVE);			
		}
	}	
	
	return false;
}

// read a 16-bit value
uint16_t ConfigData::readIntFromEEPROM (uint16_t startPos )
{
	uint16_t result = 0;
	byte *p = (byte*) &result;
	uint16_t ee = startPos;
	
	for (uint16_t i = 0; i < EE_KEY_SIZE; i++)
	{
		*p = EEPROM.read(ee++);
		p++;
	}
	return result;	
}

// read a 32-bit value
uint32_t ConfigData::readLongFromEEPROM (uint16_t startPos )
{
	uint32_t result = 0;
	byte *p = (byte*) &result;
	uint16_t ee = startPos;
	
	for (int i = 0; i < 4; i++)
	{
		*p = EEPROM.read(ee++);
		p++;
	}
	return result;
}


String ConfigData::readStringFromEEPROM(uint16_t startPos, uint16_t maxLen )
{
	const uint16_t BUFFER_LEN = 50;
	char result [BUFFER_LEN];
	byte *p = (byte*)&result[0];
	
	uint16_t ee = startPos;
	if (maxLen >= BUFFER_LEN)
	{
		maxLen = BUFFER_LEN - 1;
	}
	
	uint16_t i;
	for (i = 0; i < maxLen; i++)
	{
	  *p = EEPROM.read(ee++);
	  if (*p == 0)
	  {
		  break;
	  }
	  else{
		  p++;
	  }
	}
	
	if(i == maxLen && result[i] != 0)
		result[i+1] = 0;
	
	return String(&result[0]);
}

ConfigData configData;