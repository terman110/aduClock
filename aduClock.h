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

// VARIABLES
//  Bypass display output
bool bypassDisp = false;
//  How many cycles passed without a redraw?
unsigned long missedRefreshCount = 0;

// FUNCTIONS
//  Init MCU
void setup();
//  Execution loop
void loop();

#endif // __ADUCLOCK_H__


