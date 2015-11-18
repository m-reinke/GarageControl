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

#ifdef CC3000SHIELD
#include <Adafruit_CC3000.h>
#endif

struct t_eeprom_config
{
	unsigned int startKey;
	unsigned int configVersion;
	bool useDHCP;
	unsigned long ipAddress, netmask, gateway, dns;
	byte macAddress[EE_MAC_SIZE];
	char wlanKey[EE_CHAR_KEY_SIZE];
	char ssid[EE_CHAR_KEY_SIZE];
	byte sipKey[EE_SIP_KEY_SIZE];
	unsigned int endKey;
};


const char configFileName[] = "ARDUINO.CFG";


void ConfigData::begin(LedBase *led)
{
	this->led = led;
	pinMode(PIN_SDCARD, OUTPUT);	
		
	bool success = this->loadConfigFromSdCard();
		
	if (success){
Serial.println("Loaded from SdCard");		
		this->printConfig();
		return;
	}
	
	success = this->loadConfigFromEEPROM();
	
	if (success)
	{
Serial.println("Loaded from EEPROM");		
		this->printConfig();
	}
	else
	{
		led->SetLed(ColorRed, ALWAYS_ON);
		while(1);
	}
	
}

void printIpAddress (unsigned long ipAddress){
	for (int i = 0; i < 4; i++)
	{
		unsigned long ipx = ipAddress >> ((3-i)*8);
		Serial.print((ipx & 0xFF));
		ipx /= 256;
		if (i < 3)
			Serial.print(".");
		else
			Serial.println(" ");
	}	
}

void ConfigData::printConfig(){
	Serial.print("configVersion "); Serial.println(this->configVersion);
	Serial.print("ip address "); printIpAddress(this->ipAddress); 
	Serial.print("net mask "); printIpAddress(this->netmask);
	Serial.print("gateway "); printIpAddress(this->gateway);
	Serial.print("dns server "); printIpAddress(this->dns);

	Serial.print("ssid "); Serial.println(this->ssid);
	Serial.print("wlanKey "); Serial.println(this->wlanKey);

	Serial.println("users");
	for (unsigned int i = 0; i < EE_USER_COUNT; i++)
	{
		if (this->user_records[i].userMode > UM_INACTIVE){
			Serial.print("id: "); Serial.print(this->user_records[i].userId);
			Serial.print(", key: "); Serial.print(this->user_records[i].userKey);
			Serial.print(", mode: "); Serial.println(this->user_records[i].userMode);
		}
	}
}

void compareInt (unsigned int v1, unsigned int v2){
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

bool ConfigData::loadConfigFromEEPROM()
{
	if (!this->isEEPROMvalid())
	{
		return false;
	}
	
	t_eeprom_config buffer;
	
	byte *p = (byte*)&buffer;
	unsigned int ee = EEP_START_KEY;
	for (unsigned int i = 0; i < EE_BASE_CONFIG_LEN; i++)
	{
		*p = EEPROM.read(ee++);
		p++;
	}

	
	this->configVersion = buffer.configVersion;	
	this->useDHCP = buffer.useDHCP;
	this->ipAddress = buffer.ipAddress;	  	
	this->gateway = buffer.gateway;
	this->netmask = buffer.netmask;
	this->dns = buffer.dns;
	memcpy(this->macAddress, buffer.macAddress, sizeof(buffer.macAddress));
	memcpy(this->ssid, buffer.ssid, sizeof(buffer.ssid));
	memcpy(this->wlanKey, buffer.wlanKey, sizeof(buffer.wlanKey));
	memcpy(this->sipKey, buffer.sipKey, sizeof(buffer.wlanKey));
	
	
	p = (byte *) &this->user_records;
	ee = EEP_USERS;
	for (unsigned int i = 0; i < EE_USER_SIZE * EE_USER_COUNT; i++)
	{
		*p = EEPROM.read(ee++);
		p++;
	}
	return true;
}

bool ConfigData::isEEPROMvalid()
{
	
	unsigned int version = this->readIntFromEEPROM(EEP_CONFIG_VERSION);
	unsigned int startKey = this->readIntFromEEPROM(EEP_START_KEY);
	unsigned int endKey = this->readIntFromEEPROM(EEP_END_KEY);
	unsigned int userEndKey = this->readIntFromEEPROM(EEP_USER_END_KEY);
	
	compareInt(version, EE_CONFIG_VERSION);
	compareInt(startKey, EE_CONFIG_START_KEY);
	compareInt(endKey, EE_CONFIG_END_KEY);
	compareInt(userEndKey,EE_USER_END_KEY );

	bool result = (version >= EE_CONFIG_VERSION);
	result &= (startKey == EE_CONFIG_START_KEY);
	result &= (endKey == EE_CONFIG_END_KEY);
	result &= (userEndKey == EE_USER_END_KEY);
	
	return result;
}


bool ConfigData::loadConfigFromSdCard()
{ 
	// Activate SD Card
	SD.begin(PIN_SDCARD);
	
	t_eeprom_config buffer;
	t_user_record sdUser_records[EE_USER_COUNT];
	unsigned int userEndKey = 0;
	unsigned long fileSize = 0;

	byte *p_eeprom_config = (byte*)&buffer;	
	byte *p_user_config = (byte*)&sdUser_records;	
	byte *p_userEndKey = (byte*)&userEndKey;
	
	
	 // open the file for reading:
	 File cfgFile = SD.open(configFileName);
	 if (cfgFile) {
Serial.println("File opened");		 
		fileSize = cfgFile.size();
		 if (fileSize < EE_BASE_CONFIG_LEN)
		 {
Serial.print("Filesize too small "); 
			compareInt(fileSize, EE_BASE_CONFIG_LEN);
			 
			cfgFile.close();
			// Deactivate SD Card
			digitalWrite(PIN_SDCARD, HIGH);
			return false;
		 }
compareInt(sizeof(t_eeprom_config), EE_BASE_CONFIG_LEN);
		 
		 cfgFile.readBytes(p_eeprom_config, sizeof(t_eeprom_config));
		 
		 // Check if userdata is in file
		 if (fileSize > EEP_USER_END_KEY){
			 cfgFile.readBytes(p_user_config, sizeof(t_user_record) * EE_USER_COUNT);
			 cfgFile.readBytes(p_userEndKey, sizeof(unsigned int));
		 }
		 
		 // close the file:
		 cfgFile.close();
	 } 
	 else 
	 {
Serial.println("File not found");
		// Deactivate SD Card
		digitalWrite(PIN_SDCARD, HIGH);
		 return false;
	 }

	compareInt(buffer.configVersion, EE_CONFIG_VERSION);
	compareInt(buffer.startKey, EE_CONFIG_START_KEY);
	compareInt(buffer.endKey, EE_CONFIG_END_KEY);

	bool result = (buffer.configVersion >= EE_CONFIG_VERSION) && (buffer.startKey == EE_CONFIG_START_KEY) && (buffer.endKey == EE_CONFIG_END_KEY);
	bool hasUsers = (userEndKey == EE_USER_END_KEY);

	if (result)
	{
Serial.println("File config consistent");
		
		
		//write content of file to EEPROM
		byte *p = (byte*)&buffer;
		for (unsigned int i = 0; i < EE_CONFIG_LEN; i++)
		{
			EEPROM.write (i, *p);
			p++;
		}	
		
		this->configVersion = buffer.configVersion;
		this->useDHCP = buffer.useDHCP;
		this->ipAddress = buffer.ipAddress;
		this->gateway = buffer.gateway;
		this->netmask = buffer.netmask;
		this->dns = buffer.dns;

		memcpy(this->macAddress, buffer.macAddress, sizeof(buffer.macAddress));
		memcpy(this->ssid, buffer.ssid,       sizeof(buffer.ssid));
		memcpy(this->wlanKey, buffer.wlanKey, sizeof(buffer.wlanKey));			
		memcpy(this->sipKey, buffer.sipKey,   sizeof(buffer.wlanKey));
	} 
	else{
		// Deactivate SD Card
		digitalWrite(PIN_SDCARD, HIGH);
		return result;
	}
	
	if(hasUsers){
Serial.println("Writing userdata to EEPROM");
		// write userdata to eeprom
		byte *p = (byte*)&sdUser_records;
		for (unsigned int i = 0; i < sizeof(t_user_record) * EE_USER_COUNT; i++)
		{
			EEPROM.write (EEP_USERS + i, *p);
			p++;
		}		
		
Serial.println("Writing userkey to EEPROM");
		EEPROM.write (EEP_USER_END_KEY, *p_userEndKey++);
		EEPROM.write (EEP_USER_END_KEY+1, *p_userEndKey);
		
Serial.println("Writing user data for use");
		for (unsigned int i = 0; i < EE_USER_COUNT; i++){
			this->user_records[i] = sdUser_records[i];
		}
	}
	
	// Deactivate SD Card
	digitalWrite(PIN_SDCARD, HIGH);
		
	this->initIp();	
	
	return result;
}


void ConfigData::initIp()
{
#ifdef CC3000SHIELD
	Adafruit_CC3000 cc3000 = Adafruit_CC3000(
		ADAFRUIT_CC3000_CS,
		ADAFRUIT_CC3000_IRQ,
		ADAFRUIT_CC3000_VBAT);
	unsigned long dummy = 0;
	
	if (!cc3000.begin())
	{
		Serial.println("Couldn't begin()! Check your wiring?");
		return;
	}
	
	Serial.print("connecting '");
	Serial.print (this->ssid);
	Serial.println("'");
	Serial.print("key '");
	Serial.print (this->wlanKey);
	Serial.println("'");	
	
	
	Serial.print("IP: ");
	cc3000.printIPdotsRev (this->ipAddress);
	
//	if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY, CONNECT_RETRIES)) {
	if (!cc3000.connectToAP(this->ssid, this->wlanKey, WLAN_SECURITY)) {
		Serial.println("Couldn't connect");
		return;
	}
	Serial.println("Connected");
	
	
	if (this->useDHCP)
	{
	Serial.println("DHCP");
		
		cc3000.setDHCP();
		cc3000.getIPAddress(&this->ipAddress, &this->netmask, &this->gateway, &dummy, &this->dns);
	}
	else
	{
	    Serial.println("Static");
		cc3000.setStaticIPAddress(this->ipAddress, this->netmask, this->gateway, this->dns);
	}

	Serial.println("Disconnect");
	cc3000.disconnect();
#endif

#ifdef ETHERSHIELD

	if (connected)
		return;

	if (this->useDHCP)
	{
		Serial.println("DHCP");

		int success = Ethernet.begin(this->macAddress);
		IPAddress ip = Ethernet.localIP();

		this->ipAddress = ip;
		connected = (success == 1);
	}
	else
	{
		IPAddress ip(this->ipAddress);
		IPAddress nm(this->netmask);
		IPAddress gw(this->gateway);
		IPAddress dns(this->dns);

		Ethernet.begin(macAddress, ip, dns, gw, nm);
		connected = true;
	}


#endif
}

bool ConfigData::loadUser( unsigned int userId, t_user_record *userRecord)
{
	for (unsigned int i = 0; i < EE_USER_COUNT; i++)
	{
		if (this->user_records[i].userId == userId){
			
			userRecord = &this->user_records[i];
			
			return (this->user_records[i].userMode != UM_INACTIVE);			
		}
	}	
	
	return false;
}

unsigned int ConfigData::readIntFromEEPROM (unsigned  int startPos )
{
	unsigned int result = 0;
	byte *p = (byte*) &result;
	unsigned int ee = startPos;
	
	for (unsigned int i = 0; i < EE_KEY_SIZE; i++)
	{
		*p = EEPROM.read(ee++);
		p++;
	}
	return result;	
}

unsigned long ConfigData::readLongFromEEPROM (unsigned  int startPos )
{
	unsigned long result = 0;
	byte *p = (byte*) &result;
	unsigned int ee = startPos;
	
	for (int i = 0; i < 4; i++)
	{
		*p = EEPROM.read(ee++);
		p++;
	}
	return result;
}


String ConfigData::readStringFromEEPROM(unsigned int startPos, unsigned int maxLen )
{
	const unsigned int BUFFER_LEN = 50;
	char result [BUFFER_LEN];
	byte *p = (byte*)&result[0];
	
	unsigned int ee = startPos;
	if (maxLen >= BUFFER_LEN)
	{
		maxLen = BUFFER_LEN - 1;
	}
	
	unsigned int i;
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