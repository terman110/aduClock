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
  Serial.println("");
  Serial.println("init");

  bitmap.clear();

  tm.readFromEPROM();
  updateBitmap = true;

  Serial.println("start");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input == "on") {
      Serial.print("ok;");
      bypassDisp = false;
      updateBitmap = true;
      Serial.println("turn on");
    } else if (input == "off") {
      Serial.print("ok;");
      bypassDisp = true;
      updateBitmap = false;
      bitmap.clear();
      Serial.println("turn off");
    } else if (input == "status") {
      Serial.print("ok;");
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
      Serial.print("ok;");
      int h = input.substring(0, 2).toInt();
      int m = input.substring(3, 5).toInt();
      int s = input.substring(6, 8).toInt();
      tm.setHour(h);
      tm.setMinute(m);
      tm.setSecond(s);
      tm.writeTime();
      Serial.print("received ");
      Serial.print(h);
      Serial.print(":");
      Serial.print(m);
      Serial.print(":");
      Serial.println(s);
      updateBitmap = true;
    } else {
      Serial.println("error;");
    }
  }

  unsigned long currentMillis = millis();

  if (bypassDisp || !bitmap.DoAnimate(&tm, currentMillis, &updateBitmap))
  {
    // Blink dots
    if (!bypassDisp && (updateBitmap || currentMillis - blinkMillis >= (unsigned long)blinkDelay)) {
      blinkMillis = currentMillis;
      bitmap.dotGenerator();
    }
  
    // Update time
    if (!bypassDisp && (updateBitmap || currentMillis - updateMillis >= (unsigned long)updateDelay) && display.StartingPrint()) {
      updateMillis = currentMillis;
      if (tm.readTime())
        updateBitmap = true;
    }
  
    // Update clock
    if (updateBitmap) {
      updateBitmap = false;
      bitmap.clockGenerator(tm.Hour(), tm.Minute());
    }
  }

  display.printBitmap(bitmap);
  if (display.StartingPrint())
    updateBitmap = false;
}
