/*








*/

#include <SipHash_2_4.h>
#include <SD.h>
#include <EEPROM.h>
#include <HexConversionUtils.h>

#include "Global.h"

#ifdef ETHERSHIELD
#include <SPI.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include "IpSetupEtherShield.h"
#endif

#ifdef CC3000SHIELD
#include <SPI.h>
#include <Adafruit_CC3000_Server.h>
#include <Adafruit_CC3000.h>
#include <Dns.h>
#include <Dhcp.h>
#include <ccspi.h>
#include "IpSetupCc3000.h"
#endif


#include "WebCommand.h"
#include "GarageExec.h"

#include "Constants.h"
#include "LedControl.h"
#include "ConfigData.h"
#include "WebSession.h"
#include "GarageMotor.h"
#include "GarageSensor.h"
#include "WebParser.h"
#include "GarageExec.h"
#include "WebPages.h"

#include"HashTest.h"

LedControl led;

void setup()
{
    Serial.begin(115200);
  	Serial.println("start");

	// Deactivate Buzer
	pinMode(PIN_BUZER, OUTPUT);
	digitalWrite(PIN_BUZER, LOW);
	
	configData.begin(&led);	

	garageSensor.begin(&led);
	garageMotor.begin(&garageSensor); 

	webPages.begin(&garageSensor);
	garageExec.begin(&webPages);

	webParser.begin(&led, &webPages, &garageExec);
}

void loop()
{
	garageSensor.execute();
	led.execute();
	garageMotor.execute(); 

	webParser.execute();
}
