/*
 * Constants.h
 *
 * Created: 11.01.2014 23:27:54
 *  Author: Martin
 */ 

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <Arduino.h>
#include "Global.h"
#ifdef ETHERSHIELD
#include <Ethernet.h>
#endif

#define KEY_CMD "COMMAND"
#define KEY_CMD_PARAM "PARAM"
#define KEY_USERID "USERID"
#define KEY_USERKEY "USERKEY"
#define KEY_HASH "HASH"
#define KEY_SESSIONID "SESSIONID"
#define KEY_SESSIONIDX "MESSAGEIDX"
#define KEY_STATE "STATE"
#define KEY_RESULT "RESULT"
#define KEY_RETURN "RETURN"

#define CMD_CONNECT "CONNECT"
#define CMD_GETSTATE "GETSTATE"
#define CMD_OPEN "OPEN"
#define CMD_CLOSE "CLOSE"
#define CMD_USER "USER"

#define STATE_OK "OK"
#define STATE_NO_SESSION "NO_SESSION"
#define STATE_NO_USER "NO_USER"
#define STATE_FAILED "FAILED"
#define STATE_UNDEFINED "UNDEF"

const int PIN_BUZER = 7;
const int PIN_SDCARD = 4;
  
#ifdef CC3000SHIELD
const int LED_OFF  = LOW;
const int LED_ON   = HIGH;

const int InOpen   = 9;
const int InClosed = 8;

const int OutRed    = A0; // red 
const int OutGreen  = A1; // green 
const int OutBlue   = A2; // blue


// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

//define WLAN_SSID       "Ostpreussen"           // cannot be longer than 32 characters!
//define WLAN_PASS       "AAAAA111AAA123111AEEF44BBB"

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define CONNECT_RETRIES 3

#endif

#ifdef ETHERSHIELD
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF1, 0xE1 };
//IPAddress ip(172,16,1,177);

const int LED_ON  = HIGH;
const int LED_OFF = LOW;

const int InOpen   = 9;
const int InClosed = 8;

const int OutRed   = 3;
const int OutGreen = 2;
const int OutBlue  = 5;
#endif

#endif //__CONSTANTS_H__
