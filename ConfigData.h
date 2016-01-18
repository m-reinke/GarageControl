/* 
* ConfigData.h
*
*
*
* Created: 16.08.2014 15:24:02
* Author: Martin
*/


#ifndef __CONFIGDATA_H__
#define __CONFIGDATA_H__

#include <Arduino.h> 
#include "Global.h"
#include "LedBase.h"

///<summary>Usermode: user disabled</summary>
const uint16_t UM_INACTIVE = 0;
///<summary>Usermode: user is regular user</summary>
const uint16_t UM_USER  = 1;
///<summary>Usermode: user is admin user</summary>
const uint16_t UM_ADMIN = 2;

///<summary>Session mode - not used at the moment</summary>
const uint16_t SM_GUEST = 0;
///<summary>Session mode - not used at the moment</summary>
const uint16_t SM_USER = 1;
///<summary>Session mode - not used at the moment</summary>
const uint16_t SM_ADMIN = 2;

///<summary>Size of key values</summary>
const uint16_t EE_KEY_SIZE        = 2;
///<summary>Size of MAC-Address</summary>
const uint16_t EE_MAC_SIZE        = 6;
///<summary>Size of secret SIP key</summary>
const uint16_t EE_SIP_KEY_SIZE    = 16;
///<summary>Maximum number of users</summary>
const uint16_t EE_USER_COUNT      = 10;
///<summary>Size of text fields in config file</summary>
const uint16_t EE_CHAR_KEY_SIZE	  = 30;

/* not used anymore to calculate offsets
const uint16_t EE_USER_SIZE	      = 12;
const uint16_t EEP_START_KEY      = 0;
const uint16_t EEP_CONFIG_VERSION = EEP_START_KEY + EE_KEY_SIZE;
const uint16_t EEP_USE_DHCP       = EEP_CONFIG_VERSION + EE_KEY_SIZE;
const uint16_t EEP_IP_ADDRESS     = EEP_USE_DHCP  + 1;
const uint16_t EEP_GATEWAY        = EEP_IP_ADDRESS + EE_KEY_SIZE * 2;
const uint16_t EEP_NET_MASK       = EEP_IP_ADDRESS + EE_KEY_SIZE * 4;
const uint16_t EEP_DNS_SERVER     = EEP_IP_ADDRESS + EE_KEY_SIZE * 6;
const uint16_t EEP_MAC_ADDRESS    = EEP_IP_ADDRESS + EE_KEY_SIZE * 8;
const uint16_t EEP_WLAN_KEY       = EEP_MAC_ADDRESS + EE_MAC_SIZE;
const uint16_t EEP_SSID		      = EEP_WLAN_KEY + EE_CHAR_KEY_SIZE;
const uint16_t EEP_SIP_KEY        = EEP_SSID + EE_CHAR_KEY_SIZE;
const uint16_t EEP_END_KEY        = EEP_SIP_KEY + EE_SIP_KEY_SIZE;
const uint16_t EEP_USERS		  = EEP_END_KEY + EE_KEY_SIZE;
const uint16_t EEP_USER_END_KEY   = EEP_USERS + EE_USER_COUNT * EE_USER_SIZE;
*/

///<summary>Current version of config structure</summary>
const uint16_t EE_CONFIG_VERSION    = 3;
///<summary>For verification: First two bytes of config structure</summary>
const uint16_t EE_CONFIG_START_KEY  = 0xF1A5;
///<summary>For verification: Last two bytes of basic config structure</summary>
const uint16_t EE_CONFIG_END_KEY    = 0x5A1F;
///<summary>For verification: Final two bytes of config structure</summary>
const uint16_t EE_USER_END_KEY      = 0xA5F1;

#pragma pack
///<summary>User record</summary>
///<seealso cref="t_eeprom_config" />
struct t_user_record {
  ///<summary>User Id, random 16 bit value</summary>
  uint16_t userId;
  ///<summary>User password - not saved on client devices</summary>
  uint16_t userKey;
  ///<summary>User mode Non/User/Admin</summary> one of UM_INACTIVE/UM_USER/UM_ADMIN
  uint16_t userMode;
};

#pragma pack
///<summary>Configuration record</summary>
///Binary structure of config file from SD card or EEPROM layout 
struct t_eeprom_config
{
	///<summary>Contains start key for verification purpose</summary>
	///<seealso cref = "EE_CONFIG_START_KEY" / >	
	uint16_t startKey;

	///<summary>Contains current version of this record for verification purpose</summary>
	///seealso cref = "EE_CONFIG_VERSION" / >	
	uint16_t configVersion;

	///<summary>Whether to use DHCP to get IP address</summary>
	///Ignored, if define ENABLE_DHCP is not set
	bool useDHCP;

	///<summary>IP address</summary>
	///has to be set for fixed IP address - otherwise filled in RAM at initialization	
	uint32_t ipAddress;

	///<summary>Net mask</summary>
	///has to be set for fixed IP address - otherwise filled in RAM at initialization
	uint32_t netmask;

	///<summary>Gateway IP address</summary>
	///has to be set for fixed IP address - otherwise filled in RAM at initialization	
	uint32_t gateway;

	///<summary>DNS server IP address</summary>
	///has to be set for fixed IP address - otherwise filled in RAM at initialization	
	uint32_t dns;

	///<summary>MAC address</summary>
	///has to be set for fixed IP address - otherwise filled in RAM at initialization	
	byte macAddress[EE_MAC_SIZE];

	///<summary>WiFi password</summary>
	///Not used with CC3000
	char wlanKey[EE_CHAR_KEY_SIZE];

	///<summary>WiFi SSID</summary>
	///only used for WiFi shield(s)
	char ssid[EE_CHAR_KEY_SIZE];

	///<summary>secret key for SipHash</summary>
	///only used for WiFi shield(s)
	uint8_t sipKey[EE_SIP_KEY_SIZE];

	///<summary>Contains basic configuration end key for verification purpose</summary>
	///<seealso cref = "EE_CONFIG_END_KEY" / >
	uint16_t endKey;

	///<summary>List of active users</summary>
	///<seealso cref="t_user_record" />
	t_user_record users[EE_USER_COUNT];

	///<summary>Contains end key for verification purpose</summary>
	///<seealso cref = "EE_CONFIG_END_KEY" / >
	uint16_t userEndKey;
};

const uint16_t EE_CONFIG_LEN = sizeof(t_eeprom_config);
const uint16_t EE_USER_CONFIG_LEN = sizeof(t_user_record);
const uint16_t EE_BASE_CONFIG_LEN = EE_CONFIG_LEN - EE_USER_CONFIG_LEN - sizeof(uint16_t);
const int EE_USER_OFFSET = (int)offsetof(struct t_eeprom_config, users);

///<summary>Prints ip address for debugging</summary>
///printing to Serial.
void printIpAddress(uint32_t ipAddress);

///<summary>Prints MAC address for debugging</summary>
///printing to Serial.
void printMacAddress(uint8_t mac[]);

///<summary>Prints SIP key for debugging</summary>
///printing to Serial.
void printSipKey(uint8_t key[]);

///<summary>Configuration class</summary>
///Handling loading and saving of local configuration
class ConfigData
{
//variables
public:
	///<summary>Contains current version of this record for verification purpose</summary>
	///<seealso cref = "EE_CONFIG_VERSION" />	
	uint16_t configVersion;

	///<summary>Whether to use DHCP to get IP address</summary>
	///<remarks>Ignored, if define ENABLE_DHCP is not set</remarks>
	bool useDHCP;

	///<summary>Local IP address</summary>
	///<remarks>has to be set for fixed IP address - otherwise filled in RAM at initialization</remarks>	
	uint32_t ipAddress;

	///<summary>Net mask</summary>
	///<remarks>has to be set for fixed IP address - otherwise filled in RAM at initialization</remarks>	
	uint32_t netmask;

	///<summary>Gateway IP address</summary>
	///<remarks>has to be set for fixed IP address - otherwise filled in RAM at initialization</remarks>	
	uint32_t gateway;

	///<summary>DNS server IP address</summary>
	///<remarks>has to be set for fixed IP address - otherwise filled in RAM at initialization</remarks>	
	uint32_t dns;

	///<summary>MAC address</summary>
	///<remarks>Not used with CC3000</remarks>
	byte macAddress[EE_MAC_SIZE];

	///<summary>WiFi password</summary>
	///<remarks>only used for WiFi shield(s)</remarks>
	char wlanKey[EE_CHAR_KEY_SIZE];

	///<summary>WiFi SSID</summary>
	///<remarks>only used for WiFi shield(s)</remarks>
	char ssid[EE_CHAR_KEY_SIZE];

	///<summary>Secret key for SipHash</summary>
	uint8_t sipKey[EE_SIP_KEY_SIZE];

protected:
private:
	//Only used for start up status display
	LedBase *led;

	///<summary>Array of current users</summary>
	///<seealso cref="t_user_record" />
	t_user_record user_records[EE_USER_COUNT];
	
	///<summary>Flag showing, wether the basic configuration has been changed during setup</summary>
	bool baseConfigChanged;

	//functions
public:
	ConfigData();
	~ConfigData();
	
	///<summary>Loads user record, if userId is pressent</summary>
	///<param name='userId'>User Id to search for</param>
	///<param name='userRecord'>Field to copy user data into, if it has been found</param>
	///<returns>True if user exists</returns>
	bool loadUser(uint16_t userId, t_user_record **userRecord);

	///<summary>Saves a changed user record</summary>
	///<param name='idx'>User index in array</param>
	///<param name='id'>User ID</param>
	///<param name='key'>User Password</param>
	///<param name='mode'>User mode Non / User / Admin</param>
	///<returns>true, if user could be saved</returns>
	bool setUser(uint8_t idx, uint16_t id, uint16_t key, uint16_t mode);

	///<summary>Initialize Configuration - load from SD card or EEPROM</summary>
	///<remarks>Has to be placed in setup method of sketch ino-file</remarks>
	///<param name='led'>LED object for status display</param>
	///<returns></returns>
	void begin(LedBase *led);

	///<summary>Checks if the basic configuration has been changed during setup</summary>
	///<remarks>Happens only, when configuration is loaded from file, </remarks>
	///<returns>True if config was changed</returns>
	bool isBaseConfigChanged();


	///<summary></summary>
	///<remarks></remarks>
	///<param name=''></param>
	///<returns></returns>
	bool saveUserRecord(uint8_t idx);

protected:
private:
	ConfigData(const ConfigData &c );


	///<summary>Loading configuration data from EEPROM</summary>
	///<returns>true, if EEPROM data contains valid configuration data</returns>
	bool loadConfigFromEEPROM();

	///<summary>Loading configuration data from SD card</summary>
	///<remarks>Expects file 'arduino.cfg' in SD card root directory</remarks>
	///<returns>true, if config file present and configuration data valid</returns>
	bool loadConfigFromSdCard();

	///<summary>Loading configuration file from buffer into this object</summary>
	///<param name='buffer'>config data to load</param>
	void loadFromBuffer(t_eeprom_config * buffer);

	///<summary>Checks, whether config in buffer has valid version and valid start and end keys</summary>
	///<param name='buffer'>config data to verify</param>
	///<returns>True if configuration record is valid</returns>
	bool isBufferValid(t_eeprom_config* buffer);

	///<summary>Reads a single 16 bit value from EEPROM</summary>
	///<param name='startPos'>Offset in EEPROM</param>
	///<returns>integer value</returns>
	uint16_t readIntFromEEPROM ( uint16_t startPos );

	///<summary>Reads a single 32 bit value from EEPROM</summary>
	///<param name='startPos'>Offset in EEPROM</param>
	///<returns>32 bit value</returns>
	uint32_t readLongFromEEPROM (uint16_t startPos );

	///<summary>Loads a string from EEPROM</summary>
	///<param name='startPos'>Offset in EEPROM</param>
	///<param name='maxLen'>Expected maximum string length</param>
	///<returns>String from EEPROM</returns>
	String readStringFromEEPROM (uint16_t startPos, uint16_t maxLen);

	///<summary>Print current config to Serial</summary>
	///<remarks>Reveals secret Sip key and user passwords</remarks>
	void printConfig();

	///<summary>Print one user config</summary>
	///<remarks>Reveals user password</remarks>
	///<param name='i'>Index of user in list</param>
	void printUser(uint16_t i);
}; //configData

extern ConfigData configData;


#endif //__CONFIGDATA_H__
