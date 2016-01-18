// 
// 
// 

#include "IpSetupEtherShield.h"
#ifdef ETHERSHIELD

IpSetupEtherShield::IpSetupEtherShield()
{
	ipInitializer = this;
}

void IpSetupEtherShield::begin(LedBase * led)
{
	this->led = led;
	//Only set Chip select pin to output
	pinMode(53, OUTPUT);
}

bool IpSetupEtherShield::connect()
{
	Serial.print("MAC addr ");
	printMacAddress(configData.macAddress);

	// todo: Place DHCP and static ip in the corresponding functions
#ifdef ENABLE_DHCP
	if (!configData.useDHCP)
	{
		Serial.println(F("DHCP"));

		int success = Ethernet.begin(configData.macAddress);
		connected = (success == 1);

		if (connected)
		{
			Serial.println(F("Connected with DHCP"));
		}
		
		if(!connected)
		{
			Serial.println(F("DHCP failed"));
			led->SetLed(ColorRed, ALWAYS_ON);
			while (1);
		}

	}
	else

#endif
	{
		IPAddress ip(configData.ipAddress);
		IPAddress nm(configData.netmask);
		IPAddress gw(configData.gateway);
		IPAddress dns(configData.dns);

		Ethernet.begin(configData.macAddress, ip, dns, gw, nm);
		Serial.println(F("Connected with static IP"));

		connected = true;
	}
	return connected;
}

bool IpSetupEtherShield::disconnect()
{
	return false;
}

bool IpSetupEtherShield::processDhcp()
{
	IPAddress ip = Ethernet.localIP();
	IPAddress nm = Ethernet.subnetMask();
	IPAddress gw = Ethernet.gatewayIP();
	IPAddress dn = Ethernet.dnsServerIP();

	Serial.print(F("IP "));
	Serial.println(ip);

	Serial.print(F("DNS "));
	Serial.println(dn);

	Serial.print(F("Gateway "));
	Serial.println(gw);

	Serial.print(F("Mask "));
	Serial.println(nm);

	return true;
}

bool IpSetupEtherShield::processStaticIp()
{
	return processDhcp();
}

IpSetupEtherShield ipSetup;

#endif ETHERSHIELD