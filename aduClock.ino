#pragma SERIAL_ANSWER
#include <Arduino.h>

// Libraries to use:
//  - DS1307RTC
//  - Time
//
// TODO:
//  - Use memcpy whenever possible

#include "acCharSet.h"
#include "acBitmap.h"
#include "acTime.h"
#include "acDisplay.h"
#include "aduClock.h"

// OBJECTS
//  Time object
acTime tm; 
//  Debounce button object
acDisplay display(2, 3, 4);
acBitmap bitmap;

//  Update bitmap?
bool updateBitmap = true;

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(250);
  #if SERIAL_ANSWER
  Serial.println("");
  Serial.println("init");
  #endif // SERIAL_ANSWER

  bitmap.clear();

  tm.readFromEPROM();
  updateBitmap = true;

  #if SERIAL_ANSWER
  Serial.println("start");
  #endif // SERIAL_ANSWER
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input == "on") {
      Serial.print("ok;");
      bypassDisp = false;
      updateBitmap = true;
      #if SERIAL_ANSWER
      Serial.println("turn on");
      #endif // SERIAL_ANSWER
    } else if (input == "off") {
      Serial.print("ok;");
      bypassDisp = true;
      updateBitmap = false;
      bitmap.clear();
      #if SERIAL_ANSWER
      Serial.println("turn off");
      #endif // SERIAL_ANSWER
    } else if (input == "status") {
      #if SERIAL_ANSWER
      Serial.print("ok;");
      #endif // SERIAL_ANSWER
      if (bypassDisp)
        Serial.print("on");
      else
        Serial.print("off");
      Serial.print(";");
      Serial.print(tm.Hour());
      Serial.print(":");
      Serial.print(tm.Minute());
      Serial.print(":");
      Serial.print(tm.Second());
    } else if (input.length() == 8 && input.charAt(2) == ':' && input.charAt(5) == ':') {
      #if SERIAL_ANSWER
      Serial.print("ok;");
      #endif // SERIAL_ANSWER
      int h = input.substring(0, 2).toInt();
      int m = input.substring(3, 5).toInt();
      int s = input.substring(6, 8).toInt();
      tm.setHour(h);
      tm.setMinute(m);
      tm.setSecond(s);
      tm.writeTime();
      #if SERIAL_ANSWER
      Serial.print("received ");
      Serial.print(h);
      Serial.print(":");
      Serial.print(m);
      Serial.print(":");
      Serial.println(s);
      #endif // SERIAL_ANSWER
      updateBitmap = true;
    } else {
      #if SERIAL_ANSWER
      Serial.println("error;");
      #endif // SERIAL_ANSWER
    }
  }

  bool bIsAnimation = false;
  if (!bypassDisp && !(bIsAnimation = bitmap.DoAnimate(&tm)))
  {  
    // Update time
    if (!bypassDisp && !bIsAnimation && display.StartingPrint()) {
      tm.readTime();
      updateBitmap = true;
    }
  
    if (missedRefreshCount >= 1000) {
      updateBitmap = true;
    }

    // Update clock
    if (updateBitmap) {
      updateBitmap = false;
      bitmap.dotGenerator();
      bitmap.clockGenerator(tm.Hour(), tm.Minute());
    }
  }

  if (!updateBitmap) {
    missedRefreshCount++;
  }

  display.printBitmap(bitmap);
  if (display.StartingPrint())
    updateBitmap = false;
}
