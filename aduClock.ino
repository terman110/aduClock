#include <Arduino.h>

// Libraries to use:
//  - Bounce2
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

#include <Bounce2.h>

// OBJECTS
//  Time object
acTime tm; 
//  Debounce button object
Bounce debouncer = Bounce();
acDisplay display(2, 3, 4);
acBitmap bitmap;

//  Update bitmap?
bool updateBitmap = true;

void setup()
{
  Serial.begin(9600);
  Serial.println("Start init");

  // Init button
  pinMode(bypassPin, INPUT);
  digitalWrite(bypassPin, HIGH);
  debouncer.attach(bypassPin);
  debouncer.interval(5);

  bitmap.clear();

  tm.readFromEPROM();
  updateBitmap = true;

  Serial.println("Init done");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - ifaceMillis > ifaceDelay) {
    // Check bypass button
    if ( debouncer.update()) {
      // Get the update value
      if (debouncer.read() == HIGH) {
        if (currentMillis - btnMillis >= (unsigned long)btnPressDelay ) {
          // Long button press
          switch ( ++menuState) {
            case 1:
              bitmap.clear();
              menuVal = tm.Hour();
              bitmap.menuGenerator(0, menuVal);
              bypassDisp = false;
              break;
            case 2:
              bitmap.clear();
              tm.setHour(menuVal);
              menuVal = tm.Minute();
              bitmap.menuGenerator(1, menuVal);
              break;
            case 3:
              bitmap.clear();
              tm.setMinute(menuVal);
              tm.writeTime();
              bitmap.dotGenerator();
              bitmap.clockGenerator((byte)12, (byte)34);
              break;
            case 4:
              bitmap.clear();
              bitmap.CharSet()->WriteToEPROM();
              bitmap.menuGenerator(2, (int)bitmap.GetBlink());
              break;
            case 5:
              bitmap.clear();
              menuState = 0;
              updateBitmap = true;
              break;
          }
        } else {
          // Short button press
          //Serial.prbyteln("Short press");
          switch (menuState) {
            // Blank display?
            case 0:
              if (bypassDisp) {
                bypassDisp = false;
                updateBitmap = true;
                tm.readTime();
                display.clearDisplay();
              } else {
                bypassDisp = true;
                //bitmap.clear();
                for (byte ij = 0; ij < 8; ij++)
                  display.printBitmap(bitmap);
                if (display.StartingPrint())
                  updateBitmap = false;
                bitmap.clear();
              }
              break;
            case 1:
              menuVal = (menuVal < 23) ? menuVal + 1 : 0;
              bitmap.menuGenerator(0, menuVal);
              break;
            case 2:
              menuVal = (menuVal < 59) ? menuVal + 1 : 0;
              bitmap.menuGenerator(1, menuVal);
              break;
            case 3:
              bitmap.CharSet()->next();
              bitmap.clockGenerator((byte)12, (byte)34);
              break;
            case 4:
              bitmap.SetBlink(!bitmap.GetBlink());
              bitmap.menuGenerator(2, (int)bitmap.GetBlink());
              break;
          }
        }
      } else {
        btnMillis = currentMillis;
      }
    }
  }

  if (bypassDisp || !bitmap.DoAnimate(&tm, currentMillis, &updateBitmap))
  {
    // Blink dots
    if (!bypassDisp && menuVal < 1 && (updateBitmap || currentMillis - blinkMillis >= (unsigned long)blinkDelay)) {
      blinkMillis = currentMillis;
      bitmap.dotGenerator();
    }
  
    // Update time
    if (!bypassDisp && menuVal < 1 && (updateBitmap || currentMillis - updateMillis >= (unsigned long)updateDelay) && display.StartingPrint()) {
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
