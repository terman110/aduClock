#include <Arduino.h>

#include "acBitmap.h"
#include "acGeometry.h"

acBitmap::acBitmap() { 
  m_blink= EEPROM.read(2);
}

void acBitmap::clear() {
  for ( int y = 0; y < acGeometry::ElementWidth; y++)
    for ( int x = 0; x < acGeometry::ElementsNX; x++)
      m_data[y * acGeometry::ElementsNX + x] = 0;
}
    
// Are dots blinking?
bool acBitmap::GetBlink() { return m_blink; }

// Blink dots?
void acBitmap::SetBlink(bool _blink)
{
  if (m_blink == _blink)
    return;
  m_blink = _blink;
  EEPROM.write (2, (int)m_blink);
}

void acBitmap::dotGenerator()
{
  m_blinkState = m_blink ? !m_blinkState : true;
  
  // upper dot
  for ( int y = 1; y <= 2; y++)
    for ( int x = 4; x >= 3; x--)
      bitWrite( m_data[y * acGeometry::ElementsNX + 1], x, m_blinkState);

  // lower dot
  for ( int y = 5; y <= 6; y++)
    for ( int x = 4; x >= 3; x--)
      bitWrite( m_data[y * acGeometry::ElementsNX + 1], x, m_blinkState);
}

void acBitmap::clockGenerator(byte h, byte m)
{
  for ( int y = 0; y < acGeometry::ElementWidth; y++) {
    for ( int x = 0; x < acCharSet::NumberWidth; x++)
    {
      bitWrite(   m_data[y * acGeometry::ElementsNX],   7 - x,       bitRead( (h / 10 <= 0) ? 0 : m_char.get(h / 10, y / 2), 7 - x - (y % 2 * 4)));
      if ( x < 3)
        bitWrite( m_data[y * acGeometry::ElementsNX],   7 - x - 5,   bitRead(                     m_char.get(h % 10, y / 2), 7 - x - (y % 2 * 4)));
      else
        bitWrite( m_data[y * acGeometry::ElementsNX + 1], 7 - x + 3, bitRead(                     m_char.get(h % 10, y / 2), 7 - x - (y % 2 * 4)));
      if ( x < 1)
        bitWrite( m_data[y * acGeometry::ElementsNX + 1], x,         bitRead(                     m_char.get(m / 10, y / 2), 7 - x - (y % 2 * 4)));
      else
        bitWrite( m_data[y * acGeometry::ElementsNX + 2], 7 - x + 1, bitRead(                     m_char.get(m / 10, y / 2), 7 - x - (y % 2 * 4)));
      bitWrite(   m_data[y * acGeometry::ElementsNX + 2], 7 - x - 4, bitRead(                     m_char.get(m % 10, y / 2), 7 - x - (y % 2 * 4)));
    }
  }
}

void acBitmap::menuGenerator(int menu, int val) {
  char charSel;
  switch(menu)
  {
    case 0: charSel = 'H'; break;
    case 1: charSel = 'M'; break;
    case 2: charSel = 'B'; break;
  }
  
  for ( int y = 0; y < acGeometry::ElementWidth; y++) {
    for ( int x = 0; x < acCharSet::NumberWidth; x++)
    {
      m_data[y * acGeometry::ElementsNX] = m_char.get(charSel, y);
      if ( x < 1)
        bitWrite( m_data[y * acGeometry::ElementsNX + 1], x,         bitRead( (val / 10 <= 0) ? 0 : m_char.get(val / 10, y / 2), 7 - x - (y % 2 * 4)));
      else
        bitWrite( m_data[y * acGeometry::ElementsNX + 2], 7 - x + 1, bitRead( (val / 10 <= 0) ? 0 : m_char.get(val / 10, y / 2), 7 - x - (y % 2 * 4)));
      bitWrite(   m_data[y * acGeometry::ElementsNX + 2], 7 - x - 4, bitRead(                       m_char.get(val % 10, y / 2), 7 - x - (y % 2 * 4)));
    }
  }
}

// Space Invaders
const byte SI[2][24*8/8] = {
  {
  B01000010, B01010100, B00011000,
  B00100100, B00101010, B00111100,
  B00111100, B01010100, B01011010,
  B01011010, B00101010, B11111111,
  B01111110, B01010100, B11111111,
  B10111101, B00000000, B00100100,
  B10100101, B00000000, B01000010,
  B00111100, B00011000, B00100100
  }
  ,
  {
  B01000010, B00101010, B00011000,
  B00100100, B01010100, B00111100,
  B10111101, B00101010, B01111110,
  B11011011, B01010100, B11011011,
  B01111110, B00101010, B11111111,
  B00111100, B00000000, B00100100,
  B00100100, B00000000, B01011010,
  B11100111, B00011000, B10100101
  }
};

// Nyan Cat
const byte NC[2][24*8/8] = {
  {
  B00000000, B00000001, B00000010,
  B11001100, B01111101, B10000110,
  B00110011, B10100010, B01111001,
  B10011001, B10000110, B10000101,
  B01100110, B10010010, B00000001,
  B00110011, B10100001, B01111010,
  B11001100, B01111100, B11111100,
  B00000000, B00100100, B00000000
  }
  ,
  {
  B00000000, B01111100, B00000000,
  B10011001, B10100001, B00000010,
  B01100110, B10000101, B10000110,
  B00110011, B10010010, B01111001,
  B11001100, B10100010, B10000101,
  B01100110, B01111110, B00110001,
  B10011001, B01001001, B00110010,
  B00000000, B00000000, B11111100
  }
};

bool acBitmap::DoAnimate(acTime *tm, unsigned long currentMillis, bool* updateBitmap)
{
  if (!m_animate && currentMillis - m_aniStartMilli >= 60000 && (tm->Minute() == 0 || tm->Minute() == 15 || tm->Minute() == 30 || tm->Minute() == 45))
  //if (!m_animate) // DEBUG MODE
  {
    m_animate = true;
    m_aniMode = m_aniMode == 0 ? 1 : 0;
    m_aniI = 0;
    m_aniStartMilli = currentMillis;
    m_aniBlinkMillis = currentMillis;
  }

  if(m_animate && currentMillis - m_aniStartMilli >= 5000)
  {
    m_animate = false;
    this->clear();
    *updateBitmap = true;
  }

  if (!m_animate)
    return false;

  if (currentMillis - m_aniBlinkMillis >= (unsigned long)250 ) {
    m_aniI = m_aniI == 0 ? 1 : 0;
    m_aniBlinkMillis = currentMillis;
  }
    
  byte *data;
  switch(m_aniMode)
  {
    case 0:  // Space Invaders
      data = (byte*)SI[m_aniI];
      break;
    case 1:  // Nyan Cat
      data = (byte*)NC[m_aniI];
      break;
  }
  memcpy(this->RawData(), data, acGeometry::Width * acGeometry::Height / 8);
  
  m_aniMillis = currentMillis;
  return true;
}

