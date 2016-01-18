// 	The IP configuration for the Adafruit CC3000 shield
// 	Mostly taken from the Adafruit example code
// 

#include "IpSetupCc3000.h"
#ifdef CC3000SHIELD

Adafruit_CC3000 cc3000Shield = Adafruit_CC3000(ADAFRUIT_CC3000_CS,
	ADAFRUIT_CC3000_IRQ,
	ADAFRUIT_CC3000_VBAT); // you can change this clock speed but DI


IpSetupCc3000::IpSetupCc3000()
{
//	ipSetup = this;
}

void IpSetupCc3000::begin(LedBase * led)
{
	Serial.println(F("Setup cc3000 start"));

	IpSetupBase::begin(led);

	// run initialization of the shield
	if (!cc3000Shield.begin())
	{
		Serial.println(F("Couldn't begin()! Check your wiring?"));
		led->SetLed(ColorRed, ALWAYS_ON);
		while (1);
	}

	// Can stop here, if the arduino was not initialized from SD card
	// Basic IP configuration will be taken from the cc3000 eeprom
	// Make shure you start from SD card, when you cc3000 is plugged on this
	// Arduino for the first time
	if (!configData.isBaseConfigChanged())
	{
		return;
	}

	/* Delete any old connection data on the module */
	Serial.println(F("Deleting old connection profiles"));
	if (!cc3000Shield.deleteProfiles()) {
		Serial.println(F("Failed!"));
		while (1);
	}

	// DHCP or static IP configuration has to be done here, because 
	// it is done before connecting the WiFi to the AP

	bool success;
#ifdef ENABLE_DHCP
	if (configData.useDHCP)
	{
		Serial.println(F("Setting DHCP"));
		success = cc3000Shield.setDHCP();
		if (!success)
		{
			Serial.println(F("Setting DHCP failed"));
		}
	}
	else
#endif ENABLE_DHCP
	{
		uint32_t ipAddress, netmask, gateway, dhcpserv, dns;

		ipAddress = rev(configData.ipAddress);
		netmask   = rev(configData.netmask);
		gateway   = rev(configData.gateway);
		dns       = rev(configData.dns);
		
		/*
		uint32_t ipAddress = cc3000Shield.IP2U32(172, 16, 1, 177);
		uint32_t netmask = cc3000Shield.IP2U32(255, 255, 255, 0);
		uint32_t gateway = cc3000Shield.IP2U32(172, 16, 1, 1);
		uint32_t dns = cc3000Shield.IP2U32(172, 16, 1, 1);
		*/
		Serial.println(F("Setting static IP"));
		Serial.print(F("IP Addr: ")); printIpAddress(ipAddress);
		Serial.print(F("Netmask: ")); printIpAddress(netmask);
		Serial.print(F("Gateway: ")); printIpAddress(gateway);
		Serial.print(F("DNSserv: ")); printIpAddress(dns);

		success = !cc3000Shield.setStaticIPAddress(ipAddress, netmask, gateway, dns);
		if (success)
		{
			Serial.println(F("Static IP failed"));
		}
	}

	if (!success)
	{
		led->SetLed(ColorRed, ALWAYS_ON);
		while (1);
	}
}


bool IpSetupCc3000::connect()
{
	// only connect to the AP here

	Serial.print(F("connecting '"));
	//Serial.print(ssid);
	Serial.print(configData.ssid);
	Serial.println(F("'"));
	//Serial.print(F("key '"));
	Serial.print(configData.wlanKey);
	//Serial.print(wlanKey);
	Serial.println(F("'"));

	//if (!cc3000Shield.connectToAP(ssid, wlanKey, WLAN_SECURITY, 20)) {
	if (!cc3000Shield.connectToAP(configData.ssid, configData.wlanKey, WLAN_SECURITY)) {
		Serial.println(F("Couldn't connect"));
		return false;
	}
	Serial.println(F("Connected"));

	return true;
}


bool IpSetupCc3000::disconnect()
{
	Serial.println(F("Disconnecting"));
	cc3000Shield.disconnect();
	return true;
}

bool IpSetupCc3000::processStaticIp()
{
	// No initialization done here 

	Serial.print(F("IP Static: "));
	printIpAddress(configData.ipAddress);

	// instead only print current IP address of the shield
	processDhcp();
}


// cc3000 uses IP addresses in reverse order
uint32_t IpSetupCc3000::rev(uint32_t ip)
{
	uint32_t result = 0;
	for (int i = 0; i < 4; i++)
	{
		uint32_t ipx = (ip >> ((3 - i) * 8)) & 0xFF;

		result += (ipx << (i * 8));
	}
	return result;
}


bool IpSetupCc3000::processDhcp()
{
// todo consider printing the ip address, if DHCP is not enabled

#ifdef ENABLE_DHCP
	/* Wait for DHCP to complete */
	Serial.println(F("Wait for DHCP"));
	uint16_t repeated = 0;
	bool foundIp = true;
	while (!cc3000Shield.checkDHCP())
	{
		if (repeated < 40)
		{
			Serial.print("-");
			if (repeated % 2)
			{
				led->SetLed(ColorBlue, ALWAYS_ON);
			}
			repeated++;
			delay(100);
		}
		else
		{
			foundIp = false;
			break;
		}
		delay(100); 
	}

	if (!foundIp)
	{
		Serial.println(F("Unable to get DHCP!"));
		Serial.print(repeated);
		Serial.println(F(" atempts"));
		led->SetLed(ColorRed, ALWAYS_ON);
		while (1);
	}

	Serial.println(F("get DHCP IP"));
	uint32_t ipAddress, netmask, gateway, dhcpserv, dns;
	repeated = 0;
	foundIp = true;
	do
	{
		foundIp = cc3000Shield.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dns);

		if (foundIp)
		{
			configData.ipAddress = rev(ipAddress);
			configData.netmask = rev(netmask);
			configData.gateway = rev(gateway);
			configData.dns = rev(dns);
			break;
		}

		if (repeated < 40)
		{
			Serial.print(".");
			if (repeated % 2)
			{
				led->SetLed(ColorBlue, ALWAYS_ON);
			}
			repeated++;
			delay(100);
		}
		else
		{
			break;
		}
	} while (!foundIp);

	led->AllOff();

	if (!foundIp)
	{
		Serial.println(F("Unable to retrieve the IP Address!\r\n"));
		Serial.print(repeated);
		Serial.println(F(" atempts"));
		led->SetLed(ColorRed, ALWAYS_ON);
		while (1);
}
	else
	{
		if (repeated > 1)
		{
			Serial.print(repeated);
			Serial.println(F(" atempts"));
		}

		Serial.print(F("IP Addr: ")); printIpAddress(configData.ipAddress);
		Serial.print(F("Netmask: ")); printIpAddress(configData.netmask);
		Serial.print(F("Gateway: ")); printIpAddress(configData.gateway);
		Serial.print(F("DHCPsrv: ")); printIpAddress(dhcpserv);
		Serial.print(F("DNSserv: ")); printIpAddress(configData.dns);
		Serial.println();
	}
#endif ENABLE_DHCP
}

IpSetupCc3000 ipSetup;

#endif CC3000SHIELD
