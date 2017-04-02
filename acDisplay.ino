#include <Arduino.h>

#include "acDisplay.h"

acDisplay::acDisplay(int latchPin, int clockPin, int dataPin)
{
  m_latchPin = latchPin;
  m_clockPin = clockPin;
  m_dataPin = dataPin;
  // Init display pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void acDisplay::printBitmap(acBitmap bitmap) {
  // Loop through rows
  digitalWrite(m_latchPin, 0);
  for ( int z = acGeometry::ElementsNY - 1; z >= 0; z--) { // Number of module rows
    for ( int x = acGeometry::ElementsNX - 1; x >= 0; x--) { // Number of module columns
      shiftOut(m_dataPin, m_clockPin, MSBFIRST, rb( bitmap.RawData()[m_cRow * acGeometry::ElementsNX + x + z * acGeometry::ElementsNX]) ^ 0xFF); // Column
      shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << m_cRow);     // Row
    }
  }
  digitalWrite(m_latchPin, 1);
  if ( ++m_cRow > acGeometry::ElementHeight)
    m_cRow = 0;
}

// Reverse byte order
byte acDisplay::rb(byte x)
{
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;
}

void acDisplay::clearDisplay() {
  // Loop through rows
  digitalWrite(m_latchPin, 0);
  for ( int z = acGeometry::ElementsNY - 1; z >= 0; z--) { // Number of module rows
    for ( int x = acGeometry::ElementsNX - 1; x >= 0; x--) { // Number of module columns
      shiftOut(m_dataPin, m_clockPin, MSBFIRST, 0 ^ 0xFF); // Column
      shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << m_cRow);     // Row
    }
  }
  digitalWrite(m_latchPin, 1);
}
