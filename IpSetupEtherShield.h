
#ifndef _IPSETUPETHERSHIELD_h
#define _IPSETUPETHERSHIELD_h

#include "Global.h"

#ifdef ETHERSHIELD

#include <arduino.h>
#include <Ethernet.h>

#include "LedBase.h"
#include "ConfigData.h"
#include "IpSetupBase.h"

class IpSetupEtherShield : public IpSetupBase
{

public:
	IpSetupEtherShield();
	void begin(LedBase *led);

protected:
	bool connect();
	bool disconnect();

	bool processDhcp();
	bool processStaticIp();
private:


};

extern IpSetupEtherShield ipSetup;

#endif ETHERSHIELD

#endif _IPSETUPETHERSHIELD_h

