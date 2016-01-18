// IpConfiguration.h

#ifndef _IPSETUPCC3000_h
#define _IPSETUPCC3000_h

#include "Global.h"
#ifdef CC3000SHIELD

#include "arduino.h"

#include <Adafruit_CC3000.h>

#include "LedBase.h"
#include "ConfigData.h"
#include "IpSetupBase.h"


/// <summary>
/// Initialization procedure for the Adafruit cc3000 WiFi shield
/// This procedure is not yet fully tested!!!!
/// </summary>
/// <seealso cref="IpSetupBase" />
class IpSetupCc3000 : public IpSetupBase
{
public:

private:
	/// <summary>
	/// Flag showing, that an ip address
	/// </summary>
	bool hasIpAddress;

public:
	IpSetupCc3000();
	/// <summary>
	/// Initialization - place in the setup function of the ino file.
	/// </summary>
	/// <param name="webPages">The web pages.</param>
	void begin(LedBase *led);

protected:
	/// <summary>
	/// Connects the shield to the AP.
	/// </summary>
	/// <returns>true if successfull</returns>
	bool connect();


	/// <summary>
	/// Disconnects the WiFi.
	/// </summary>
	/// <returns></returns>
	bool disconnect();

	bool processDhcp();
	bool processStaticIp();

	uint32_t rev(uint32_t ip);
};

extern IpSetupCc3000 ipSetup;
extern Adafruit_CC3000 cc3000Shield;

#endif CC3000SHIELD

#endif _IPSETUPCC3000_h

