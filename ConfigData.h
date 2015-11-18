/* 
* ConfigData.h
*
* Created: 16.08.2014 15:24:02
* Author: Martin
*/


#ifndef __CONFIGDATA_H__
#define __CONFIGDATA_H__

#include <Arduino.h> 
#include "Global.h"
#include "LedBase.h"

const unsigned int UM_INACTIVE = 0;
const unsigned int UM_USER  = 1;
const unsigned int UM_ADMIN = 2;

const unsigned int SM_GUEST = 0;
const unsigned int SM_USER = 1;
const unsigned int SM_ADMIN = 2;

const unsigned int EE_KEY_SIZE        = 2;
const unsigned int EE_MAC_SIZE        = 6;
const unsigned int EE_SIP_KEY_SIZE    = 16;
const unsigned int EE_USER_COUNT      = 10;
const unsigned int EE_USER_SIZE	      = 12;
const unsigned int EE_CHAR_KEY_SIZE	  = 30;

const unsigned int EEP_START_KEY      = 0;
const unsigned int EEP_CONFIG_VERSION = EEP_START_KEY + EE_KEY_SIZE;
const unsigned int EEP_USE_DHCP       = EEP_CONFIG_VERSION + EE_KEY_SIZE;
const unsigned int EEP_IP_ADDRESS     = EEP_USE_DHCP  + 1;
const unsigned int EEP_GATEWAY        = EEP_IP_ADDRESS + EE_KEY_SIZE * 2;
const unsigned int EEP_NET_MASK       = EEP_IP_ADDRESS + EE_KEY_SIZE * 4;
const unsigned int EEP_DNS_SERVER     = EEP_IP_ADDRESS + EE_KEY_SIZE * 6;
const unsigned int EEP_MAC_ADDRESS    = EEP_IP_ADDRESS + EE_KEY_SIZE * 8;
const unsigned int EEP_WLAN_KEY       = EEP_MAC_ADDRESS + EE_MAC_SIZE;
const unsigned int EEP_SSID		      = EEP_WLAN_KEY + EE_CHAR_KEY_SIZE;
const unsigned int EEP_SIP_KEY        = EEP_SSID + EE_CHAR_KEY_SIZE;
const unsigned int EEP_END_KEY        = EEP_SIP_KEY + EE_SIP_KEY_SIZE;
const unsigned int EEP_USERS		  = EEP_END_KEY + EE_KEY_SIZE;
const unsigned int EEP_USER_END_KEY   = EEP_USERS + EE_USER_COUNT * EE_USER_SIZE;


const unsigned int EE_CONFIG_VERSION    = 3;
const unsigned int EE_CONFIG_START_KEY  = 0xF1A5;
const unsigned int EE_CONFIG_END_KEY    = 0x5A1F;
const unsigned int EE_USER_END_KEY      = 0xA5F1;

const unsigned int EE_BASE_CONFIG_LEN = EEP_USERS;
const unsigned int EE_CONFIG_LEN = EEP_END_KEY + EE_KEY_SIZE;


struct t_user_record {
  unsigned int userId;
  unsigned int userKey;
  unsigned int userMode;
};


class ConfigData
{
//variables
public:
  unsigned int configVersion;
  bool useDHCP;
  unsigned long ipAddress, netmask, gateway, dns;  		
  byte macAddress[EE_MAC_SIZE];
  char wlanKey[EE_CHAR_KEY_SIZE];
  char ssid[EE_CHAR_KEY_SIZE];
  unsigned char sipKey[EE_SIP_KEY_SIZE];
  bool connected;
protected:
private:
  t_user_record user_records[EE_USER_COUNT];
  LedBase *led;
//functions
public:
	ConfigData();
	~ConfigData();
	
	bool loadUser(unsigned int userId, t_user_record *userRecord);
	void begin(LedBase *led);
	void initIp();


protected:
private:
	ConfigData( const ConfigData &c );
	ConfigData& operator=( const ConfigData &c );

	bool loadConfigFromEEPROM();
	bool loadConfigFromSdCard();
	
	bool isEEPROMvalid();
	unsigned int readIntFromEEPROM ( unsigned int startPos );
	unsigned long readLongFromEEPROM (unsigned int startPos );
	String readStringFromEEPROM (unsigned int startPos, unsigned int maxLen);
	void printConfig();
}; //configData

extern ConfigData configData;


#endif //__CONFIGDATA_H__
