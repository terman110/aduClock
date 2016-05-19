#ifndef __ADUCLOCK_H__
#define __ADUCLOCK_H__

#include <Arduino.h>

//  If defined soft serial bluetooth dongle will be used
//#define USE_BLUETOOTH
//  If defined, dots will blink
//#define BLINK

// PINS
//  Set RTC to compiler time after uploading? Increment "setTimeInd"!
const int setTimeInd = 5;
//  Display
const int latchPin = 2;
const int clockPin = 3;
const int dataPin = 4;
//  Bypass display button
const int bypassPin = 7;
//  Serial bluetooth
#ifdef USE_BLUETOOTH
const int btRX = 11;
const int btTX = 10;
#endif // USE_BLUETOOTH
//  RTC pins used by <DS1307RTC.h> (must not be specified)
// SDA(5) at A4
// SCL(6) at A5

// CONSTANTS
//  Display module dimensions
const int w_disp = 3;//(byte)(W / 8);
const int h_disp = 1;//(byte)(H / 8);
//  Pixel dimensions
const int W = w_disp*8;//16;
const int H = h_disp*8;//16;
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
//  Is button pressed?
bool btnPressed = false;
//  Bitmap array
byte bitmap[W*H/8] = {0};
//  Update bitmap?
bool updateBitmap = true;
//  Output row index
int pbY = 0;
//  State of user menu
//    0:  Clock
//    1:  Set hour
//    2:  Set minute
//    3:  Done
int menuState = 0;
//  Temporary value in menu
int menuVal = 0;

// FUNCTIONS
//  Init MCU
void setup();
//  Execution loop
void loop();
//  Generate blinking dot in bitmap buffer
void dotGenerator();
//  Generate four clock digets to bitmap buffer
void clockGenerator( int h, int m, int charset);
//  Clear bitmap buffer
void clearBitmap();
//  Bit bang bitmap buffer to LED matrix
void printBitmap();
//  Reverse byte order
byte rb( byte x);
//  Get time from compile time constant
bool getTime(const char *str);
//  Get date from compile date constant
bool getDate(const char *str);

/*
 *  ANIMATION PLAYGROUND!
 */
#include "animation.h"

bool show_clk_ani = false;
unsigned long aniClkMillis = 0;
unsigned long aniClkDelay = 30000;

int ani_index = 0;
int ani_i = 0;
unsigned long aniMillis = 0;
bool show_ani = false;
void animate( unsigned long currentTime, int ani = 0, int time_ms = 250)
{
  if( currentTime - aniMillis >= (unsigned long)time_ms ) {
    int n;
    byte *data;
    switch( ani) {
      case 0:  // Space Invaders
        n = SI_N;
        break;
      case 1:  // Nyan Cat
        n = NC_N;
        break;
    }
    
    if( ++ani_i >= n)
      ani_i = 0;
      
    switch( ani) {
      case 0:  // Space Invaders
        n = SI_N;
        data = (byte*)SI[ani_i];
        break;
      case 1:  // Nyan Cat
        data = (byte*)NC[ani_i];
        break;
    }
    memcpy( bitmap, data, W*H/8);
    
    aniMillis = currentTime;
  }
}

#endif // __ADUCLOCK_H__

