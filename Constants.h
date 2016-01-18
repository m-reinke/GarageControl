/*
 * Constants.h
 * Containing constants for communication and basic setup of arduino ports
 * Need to be replicated in client source codes
 * Created: 11.01.2014 23:27:54
 *  Author: Martin
 */ 

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <Arduino.h>
#include "Global.h"

// Command constants

#define CMD_CONNECT "CONNECT"
#define CMD_GETSTATE "GETSTATE"
#define CMD_OPEN "OPEN"
#define CMD_CLOSE "CLOSE"
#define CMD_USER "USER"

// Command parameter keys
#define KEY_CMD "COMMAND"
#define KEY_USERIDX  "USERIDX"
#define KEY_USERID   "USERID"
#define KEY_USERKEY  "USERKEY"
#define KEY_SETUSERID  "SETUSERID"
#define KEY_SETUSERKEY "SETUSERKEY"
#define KEY_USERMODE "USERMODE"
#define KEY_HASH "HASH"
#define KEY_SESSIONID "SESSIONID"
#define KEY_SESSIONIDX "MESSAGEIDX"
#define KEY_STATE "STATE"
#define KEY_RESULT "RESULT"
#define KEY_RETURN "RETURN"
#define KEY_DATA "DATA"

// Reply attributes
#define ATTR_STATE "STATUS"
#define ATTR_ID "ID"

// Reply error states
#define STATE_OK "OK"
#define STATE_NO_SESSION "NO_SESSION"
#define STATE_NO_USER "NO_USER"
#define STATE_FAILED "FAILED"
#define STATE_UNDEFINED "UNDEF"

// Buzzer - not used yet
const int PIN_BUZER = 7;
// SD-Card - usually on pin 4
const int PIN_SDCARD = 4;

//INPUT or INPUT_PULLUP, if no external pull up resistor is set
const int SENSOR_PORT_MODE = INPUT;

//LED status - allows to toggle
const int LED_OFF = LOW;
const int LED_ON = HIGH;

#ifdef CC3000SHIELD

const int InOpen = 9;	 // input reed relais OPEN
const int InClosed = 8;	 // input reed relais CLOSE

const int OutRed    = A0; // red 
const int OutGreen  = A1; // green 
const int OutBlue   = A2; // blue


// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define CONNECT_RETRIES 3

#endif

#ifdef ETHERSHIELD

const int InOpen = 9;	 // input reed relais OPEN
const int InClosed = 8;	 // input reed relais CLOSE

const int OutRed   = A2; // red 
const int OutGreen = A1; // green 
const int OutBlue  = A0; // blue
#endif

#endif //__CONSTANTS_H__
