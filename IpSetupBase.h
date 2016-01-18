// IpConfiguration.h

#ifndef _IPSETUPBASE_h
#define _IPSETUPBASE_h

#include "arduino.h"

#include "Global.h"
#include "LedBase.h"
#include "ConfigData.h"

/// <summary>
/// Abstract base class for the initialization of the 
/// Network hardware
/// </summary>
class IpSetupBase
{

protected:

	/// <summary>
	/// The led controller for setup stage - and error display
	/// </summary>
	LedBase *led;

	/// <summary>
	/// The connected state
	/// </summary>
	bool connected;
public:

	/// <summary>
	/// Initialization, place in the setup() function of the ino file.
	/// </summary>
	/// <param name="garageSensor">The LED controll object.</param>
	virtual void begin(LedBase *led);

	/// <summary>
	/// Sequence to open the network communication.
	/// </summary>
	/// <param name="keepConnected">Keep connection if set to <c>true</c>.</param>
	/// <returns>True if successful</returns>
	bool open(bool keepConnected);

	/// <summary>
	/// Determines whether the network is conncted.
	/// </summary>
	/// <returns></returns>
	bool isConncted();
protected:
	/// <summary>
	/// Connects the device to the internet.
	/// </summary>
	/// <returns>True if successful</returns>
	virtual bool connect() = 0;

	/// <summary>
	/// Disconnects the device from the network - if supported from the device
	/// </summary>
	/// <returns>True if successful</returns>
	virtual bool disconnect() = 0;

	/// <summary>
	/// Runs the DHCP query to retrieve an IP address.
	/// </summary>
	/// <returns>True if successful</returns>
	virtual bool processDhcp() = 0;


	/// <summary>
	/// Processes the static ip address configuration.
	/// </summary>
	/// <returns></returns>
	virtual bool processStaticIp() = 0;
};

extern IpSetupBase *ipInitializer;

#endif

