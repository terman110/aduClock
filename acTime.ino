//#include <Arduino.h>

#include "acTime.h"

acTime::acTime() 
{
}

int acTime::Hour() { return m_tm.Hour; }
int acTime::Minute() { return m_tm.Minute; }
int acTime::Second() { return m_tm.Second; }
int acTime::setHour(int value) { m_tm.Hour = value; }
int acTime::setMinute(int value) { m_tm.Minute = value; }
int acTime::setSecond(int value) { m_tm.Second = value; }

bool acTime::readTime()
{
  tmElements_t t_tm;
  RTC.read(t_tm);
  bool bNew = t_tm.Hour != m_tm.Hour || t_tm.Minute != m_tm.Minute;
  if (bNew)
    m_tm = t_tm;
  return bNew;
}

void acTime::writeTime()
{
  RTC.write(m_tm);
}

bool acTime::parseTimeFromString(const char *str)
{
  int Hour, Min, Sec;
  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  m_tm.Hour = Hour;
  m_tm.Minute = Min;
  m_tm.Second = Sec;
  return true;
}

void acTime::readFromEPROM()
{
  // Set time from compiler? Check EEPROM.
  if ( EEPROM.read(0) != setTimeInd) 
  {
    EEPROM.write(0, setTimeInd);
    // get the date and time the compiler was run
    if (parseTimeFromString(__TIME__))
      // and configure the RTC with this info
      writeTime();
  }
  else
  {
    readTime();
  }
}
