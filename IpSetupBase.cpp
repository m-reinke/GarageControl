// 
// 
// 

#include "IpSetupBase.h"

void IpSetupBase::begin(LedBase * led)
{
	this->led = led;
}

bool IpSetupBase::open(bool keepConnected)
{
	// define the general network setup here
	if (connected)
		// just return if already connected
		return true;

	// try connecting
	bool result = connect();
	if (!result)
	{
		led->SetLed(ColorRed, ALWAYS_ON);
		return false;
	}
	Serial.println(F("Connected"));

#ifdef ENABLE_DHCP
	// do DHCP - if enabled - and set
	if (configData.useDHCP)
	{
		result = processDhcp();
	}
	else
#endif ENABLE_DHCP
	{
		// otherwise just load the static IP configuration
		result = processStaticIp();
	}

	if (!keepConnected)
	{
		disconnect();
	}
	return true;
}

bool IpSetupBase::isConncted()
{
	return connected;
}

IpSetupBase *ipInitializer;