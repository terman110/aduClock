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
  Serial.println("Start init");

  bitmap.clear();

  tm.readFromEPROM();
  updateBitmap = true;

  Serial.println("Init done");
}

void loop() {
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
