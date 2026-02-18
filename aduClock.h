#ifndef __ADUCLOCK_H__
#define __ADUCLOCK_H__

#include <Arduino.h>

//  If defined, dots will blink
//#define BLINK

// PINS
//  RTC pins used by <DS1307RTC.h> (must not be specified)
// SDA(5) at A4
// SCL(6) at A5

// CONSTANTS
//  Delay of dot blink
const int blinkDelay = 1000;
//  Update time delay
const int updateDelay= 500;
//  Delay for byteerface byteeractions
const int ifaceDelay = 100;
//  Delay to enter advanced button state
const int btnPressDelay = 250;

// VARIABLES
//  Bypass display output
bool bypassDisp = false;
//  Blink dot state
bool blinkState = false;
//  Current blink dot millis
unsigned long blinkMillis = 0;
//  Current update display millis
unsigned long updateMillis = 0;
//  Current byteerface millis
unsigned long ifaceMillis = 0;
//  Time button is pressed
unsigned long btnMillis = 0;

// FUNCTIONS
//  Init MCU
void setup();
//  Execution loop
void loop();

#endif // __ADUCLOCK_H__


