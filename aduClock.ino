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
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int h = Serial.parseInt();
    // do it again:
    int m = Serial.parseInt();
    // do it again:
    int s = Serial.parseInt();

    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {
      Serial.print("New time received: ");
      Serial.print(h);
      Serial.print(":");
      Serial.print(m);
      Serial.print(":");
      Serial.println(s);
      tm.setHour(h);
      tm.setMinute(m);
      tm.setSecond(s);
      tm.writeTime();
      updateBitmap = true;
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
