// http://playground.arduino.cc/Code/time
// http://www.pjrc.com/teensy/td_libs_DS1307RTC.html
// SDA(5) at A4
// SCL(6) at A5

// TODO:
//  - Change bool data to binary bytes (B01100010)   !!!
//  - Use memcpy whenever possible

#include "aduClock.h"
#include "num_char.h"

#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <Bounce2.h>
#ifdef USE_BLUETOOTH
  #include <SoftwareSerial.h>
#endif // USE_BLUETOOTH
#include <EEPROM.h>

// OBJECTS
//  Time object
tmElements_t tm;
//  Softserial of bluetooth
#ifdef USE_BLUETOOTH
SoftwareSerial btSerial(btRX,btTX);
#endif // USE_BLUETOOTH
//  Debounce button object
Bounce debouncer = Bounce(); 

void setup() {
  // Init display pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Init button
  pinMode(bypassPin, INPUT);
  digitalWrite(bypassPin,HIGH);
  debouncer.attach(bypassPin);
  debouncer.interval(5);
  
  // Set time from compiler? Check EEPROM.
  if( EEPROM.read(0) != setTimeInd) {
    EEPROM.write(0, setTimeInd);
    // get the date and time the compiler was run
    if (getDate(__DATE__) && getTime(__TIME__))
      // and configure the RTC with this info
      RTC.write(tm);
  }
    
  int chartmp = EEPROM.read(1);
  if( chartmp >= 0 && chartmp < charN)
    char_index < chartmp;
  
  // Init serial bluetooth dongle
  #ifdef USE_BLUETOOTH
  btSerial.begin(9600);
  #endif // USE_BLUETOOTH
  
  clearBitmap();
  
  //Serial.begin(9600);
  /*
  unsigned long currentMillis = millis();
  unsigned long startMillis = currentMillis;
  const int startDelay = 1500*N_ANI;
  int i_ani = 0;
  while( currentMillis - startMillis < (unsigned long)startDelay)
  {
    if( currentMillis - startMillis < (unsigned long)(i_ani+1)*startDelay/N_ANI)
      ++i_ani;
    animate( currentMillis, i_ani);
    printBitmap();
    currentMillis = millis();
  }
  
  clearBitmap();
  //*/
  
  RTC.read(tm);
  updateBitmap = true;
}

void loop() {  
  unsigned long currentMillis = millis();
 
  if(currentMillis - ifaceMillis > ifaceDelay) {
    // Check bypass button
    if( debouncer.update()) {
      // Get the update value
      if( debouncer.read() == HIGH) {
        if( currentMillis - btnMillis >= (unsigned long)btnPressDelay ) {
           // Long button press
           switch( ++menuState) {
             case 1:
               clearBitmap();
               menuVal = tm.Hour;
               menuGenerator( 0, menuVal);
               bypassDisp = false;
               break;
             case 2:
               tm.Hour = menuVal;
               menuVal = tm.Minute;
               menuGenerator( 1, menuVal);
               break;
             case 3:
               tm.Minute = menuVal;
               RTC.write(tm);
               //updateBitmap = true;
               show_ani = true;
               break;
             case 4:
               show_ani = false;
               clearBitmap();               
               dotGenerator();             
               clockGenerator( (byte)12, (byte)34, (int)char_index);
               break;
             case 5:
               menuState = 0;
               clearBitmap();
               updateBitmap = true;
               EEPROM.write( 1, char_index);
               break;
           }
         } else {
           // Short button press
           //Serial.prbyteln("Short press");
           switch( menuState) {
             // Blank display?
             case 0:
               if( bypassDisp) {
                 bypassDisp = false;
                 updateBitmap = true;
                 RTC.read(tm);
               } else {
                  bypassDisp = true;
                  clearBitmap();
                  for( byte ij = 0; ij < 8; ij++)
                    printBitmap();
               } 
               break;
             case 1:
               menuVal = (menuVal < 23)?menuVal+1:0;
               menuGenerator( 0, menuVal);
               break;
             case 2:
               menuVal = (menuVal < 59)?menuVal+1:0;
               menuGenerator( 1, menuVal);
               break;
             case 3:
               ani_index = ( ani_index < N_ANI-1)?ani_index+1:0;
               break;
             case 4:
               char_index = ( char_index < charN-1)?char_index+1:0;   
               clockGenerator( (byte)12, (byte)34, (int)char_index);
               break;
           }
         }
       } else {
         btnMillis = currentMillis;
      }
    }
    
    #ifdef USE_BLUETOOTH
    while( btSerial.available()){
      switch( btSerial.read()) {
      case 't':
        btSerial.prbyte(tm.Day);
        btSerial.prbyte(".");
        btSerial.prbyte(tm.Month);
        btSerial.prbyte(".");
        btSerial.prbyte(1970+tm.Year);
        btSerial.prbyte(" ");
        btSerial.prbyte(tm.Hour);
        btSerial.prbyte(":");
        btSerial.prbyte(tm.Minute);
        btSerial.prbyte(":");
        btSerial.prbyteln(tm.Second);
        break;
      } 
    }
    #endif // USE_BLUETOOTH
  }

  if( show_ani) {
    // Show animation
    animate( currentMillis, ani_index);
  }else if( menuState < 1 && !bypassDisp && show_clk_ani) {
    if(currentMillis - aniClkMillis < (unsigned long)aniClkDelay) {
      animate( currentMillis, ani_index);
    } else {
      clearBitmap();
      show_clk_ani = false;
      updateBitmap = true;
    }
  }else if( menuState < 1 && !bypassDisp) {
    // Blink dots
    if(currentMillis - blinkMillis >= (unsigned long)blinkDelay) {
      blinkMillis = currentMillis; 
      dotGenerator();
    }

    // Update time
    if( currentMillis - updateMillis >= (unsigned long)updateDelay && pbY == 0) {
      updateMillis = currentMillis; 
      clearDisplay();
      tmElements_t t_tm;
      //RTC.read(t_tm);
      RTC.readM(t_tm);
      if( t_tm.Hour != tm.Hour || t_tm.Minute != tm.Minute) {
        tm = t_tm;
        updateBitmap = true;
        if( tm.Minute == 0 || tm.Minute == 15 || tm.Minute == 30 || tm.Minute == 45) {
          aniClkMillis = currentMillis; 
          if( ++ani_index >= N_ANI)
            ani_index = 0; 
          clearBitmap();
          show_clk_ani = true;
        }
      }
    }

    // Update clock
    if( updateBitmap) { 
      updateBitmap = false;
      clockGenerator(tm.Hour,tm.Minute,char_index);
    }
  }
  
  // Output clock
  if( !bypassDisp)
    printBitmap(); 
}

void dotGenerator() {
  #ifdef BLINK
  blinkState = blinkState?false:true;
  #else // BLINK
  blinkState = true;
  #endif // BLINK
  // upper dot
  for( int y = 1; y <= 2; y++)
    for( int x = 4; x >= 3; x--)
      bitWrite( bitmap[y*w_disp + 1], x, blinkState);
      //bitmap[y*w_disp + 1] &= ~(1 << x);
      //bitmap[y*w_disp + 1] |= blinkState << x;
      
  // lower dot
  for( int y = 5; y <= 6; y++)
    for( int x = 4; x >= 3; x--)
      bitWrite( bitmap[y*w_disp + 1], x, blinkState);
}

void clockGenerator( byte h, byte m, int charset){
  for( int y = 0; y < 8; y++) {
    for( int x = 0; x < WS; x++)
    {
      bitWrite(   bitmap[y*w_disp],   7-x,   bitRead( (h/10 <= 0)?0:NUM[charset][h/10][y/2], 7-x-(y%2*4)));
      if( x < 3)
        bitWrite( bitmap[y*w_disp],   7-x-5, bitRead( NUM[charset][h%10][y/2],               7-x-(y%2*4)));
      else
        bitWrite( bitmap[y*w_disp+1], 7-x+3, bitRead( NUM[charset][h%10][y/2],               7-x-(y%2*4)));
      if( x < 1)
        bitWrite( bitmap[y*w_disp+1], x,     bitRead( NUM[charset][m/10][y/2],               7-x-(y%2*4)));
      else
        bitWrite( bitmap[y*w_disp+2], 7-x+1, bitRead( NUM[charset][m/10][y/2],               7-x-(y%2*4)));
      bitWrite(   bitmap[y*w_disp+2], 7-x-4, bitRead( NUM[charset][m%10][y/2],               7-x-(y%2*4)));
    }
  }
}

void menuGenerator( int menu, int val) {
  for( int y = 0; y < 8; y++) {
    for( int x = 0; x < WS; x++)
    {
      bitmap[y*w_disp] = (menu == 0)?NUM_H[y]:NUM_M[y];
      if( x < 1)
        bitWrite( bitmap[y*w_disp+1], x,     bitRead( (val/10 <= 0)?0:NUM[0][val/10][y/2],  7-x-(y%2*4)));
      else
        bitWrite( bitmap[y*w_disp+2], 7-x+1, bitRead( (val/10 <= 0)?0:NUM[0][val/10][y/2],  7-x-(y%2*4)));
      bitWrite(   bitmap[y*w_disp+2], 7-x-4, bitRead( NUM[0][val%10][y/2],                  7-x-(y%2*4)));
    }
  }
}

void clearBitmap(){
  for( int y = 0; y < 8; y++) {
    for( int x = 0; x < w_disp; x++) {
      bitmap[y*w_disp+x] = 0;
    }
  }
}

void printBitmap(){
  // Loop through rows
  digitalWrite(latchPin, 0);
  for( int z = h_disp-1; z >= 0; z--) {  // Number of module rows 
    for( int x = w_disp-1; x >= 0; x--) {  // Number of module columns
      shiftOut(dataPin, clockPin, MSBFIRST, rb( bitmap[pbY*w_disp + x + z*w_disp]) ^ 0xFF); // Column
      shiftOut(dataPin, clockPin, MSBFIRST, 1 << pbY);     // Row
    }
  } 
  digitalWrite(latchPin, 1);
  if( ++pbY > 7) {
    pbY = 0;
    updateBitmap = false;
  }
}

void clearDisplay(){
  // Loop through rows
  digitalWrite(latchPin, 0);
  for( int z = h_disp-1; z >= 0; z--) {  // Number of module rows 
    for( int x = w_disp-1; x >= 0; x--) {  // Number of module columns
      shiftOut(dataPin, clockPin, MSBFIRST, 0 ^ 0xFF); // Column
      shiftOut(dataPin, clockPin, MSBFIRST, 1 << pbY);     // Row
    }
  } 
  digitalWrite(latchPin, 1);
}

// Reverse byte order
byte rb( byte x)
{
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;
  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}


