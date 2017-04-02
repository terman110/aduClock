//#include <Arduino.h>

#include "acTime.h"

acTime::acTime() 
{
}

int acTime::Hour() { return m_tm.Hour; }
int acTime::Minute() { return m_tm.Minute; }
int acTime::Second() { return m_tm.Second; }
int acTime::Year() { return m_tm.Year; }
int acTime::Month() { return m_tm.Month; }
int acTime::Day() { return m_tm.Day; }
int acTime::setHour(int value) { m_tm.Hour = value; }
int acTime::setMinute(int value) { m_tm.Minute = value; }
int acTime::setSecond(int value) { m_tm.Second = value; }
int acTime::setYear(int value) { m_tm.Year = value; }
int acTime::setMonth(int value) { m_tm.Month = value; }
int acTime::setDay(int value) { m_tm.Day = value; }

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

bool acTime::parseDateFromString(const char *str)
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
  m_tm.Day = Day;
  m_tm.Month = monthIndex + 1;
  m_tm.Year = CalendarYrToTm(Year);
  return true;
}

void acTime::readFromEPROM()
{
  // Set time from compiler? Check EEPROM.
  if ( EEPROM.read(0) != setTimeInd) 
  {
    EEPROM.write(0, setTimeInd);
    // get the date and time the compiler was run
    if (parseDateFromString(__DATE__) && parseTimeFromString(__TIME__))
      // and configure the RTC with this info
      writeTime();
  }
  else
  {
    readTime();
  }
}
