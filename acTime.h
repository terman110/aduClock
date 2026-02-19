#ifndef __AC_TIME_H__
#define __AC_TIME_H__

#include <Arduino.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <EEPROM.h>

class acTime
{
protected:
  // Set RTC to compiler time after uploading? Increment "setTimeInd"!
  const int setTimeInd = 5;

  // Time object
  tmElements_t m_tm;
  
public:
  // Constructor
  acTime();

  // Get hour
  int Hour();
  // Get minute
  int Minute();
  // Get second
  int Second();
  // Set hour
  int setHour(int value);
  // Set minute
  int setMinute(int value);
  // Set second
  int setSecond(int value);

  // Read time from DS1307 IC
  bool readTime();

  // Write time to DS1307 IC
  void writeTime();
  
  //  Get time from compile time constant
  bool parseTimeFromString(const char *str);

  // Read flag from EPROM
  void readFromEPROM();
};

#endif // __AC_TIME_H__
