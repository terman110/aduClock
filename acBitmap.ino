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

void acBitmap::clear(byte *data) {
  for ( int y = 0; y < acGeometry::ElementWidth; y++)
    for ( int x = 0; x < acGeometry::ElementsNX; x++)
      data[y * acGeometry::ElementsNX + x] = 0;
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

// Space Invaders
const byte SI[2][24*8/8] = {
  {
  B01000010, B00000000, B00011000,
  B00100100, B00000000, B00111100,
  B00111100, B00000000, B01011010,
  B01011010, B00000000, B11111111,
  B01111110, B00000000, B11111111,
  B10111101, B00000000, B00100100,
  B10100101, B00000000, B01000010,
  B00111100, B00000000, B00100100
  }
  ,
  {
  B01000010, B00000000, B00011000,
  B00100100, B00000000, B00111100,
  B10111101, B00000000, B01111110,
  B11011011, B00000000, B11011011,
  B01111110, B00000000, B11111111,
  B00111100, B00000000, B00100100,
  B00100100, B00000000, B01011010,
  B11100111, B00000000, B10100101
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
  B01100110, B01111110, B00000001,
  B10011001, B01001001, B01111010,
  B00000000, B00000000, B11111100
  }
};

byte ANI[24*8/8] =
  {
  B00010000, B00010000, B00001000,
  B00010000, B00010000, B00001000,
  B00010000, B00010000, B00001000,
  B00010000, B00010000, B00001000,
  B00010000, B00010000, B00001000,
  B00010000, B00010000, B00001000,
  B00010000, B00010000, B00001000,
  B00010000, B00010000, B00001000
  }
;

bool acBitmap::DoAnimate(acTime *tm, unsigned long currentMillis, bool* updateBitmap)
{
  bool bAnyChanged = false;

  if (!m_animate && currentMillis - m_aniStartMilli >= 60000 && (tm->Minute() == 0 || tm->Minute() == 15 || tm->Minute() == 30 || tm->Minute() == 45))
  // if (!m_animate) // DEBUG MODE
  {
    Serial.println("Start Animation");
    m_animate = true;
    m_aniMode = random(0, 5 * m_aniNum) / 5;
    m_aniI = 0;
    m_aniStartMilli = currentMillis;
    m_aniBlinkMillis = currentMillis;
    m_aniFrameMillis = 250;
    bAnyChanged = true;
  }

  if(m_animate && currentMillis - m_aniStartMilli >= 5000)
  {
    Serial.println("Stop Animation");
    m_animate = false;
    this->clear();
    *updateBitmap = true;
  }

  if (!m_animate)
    return false;

  if (currentMillis - m_aniBlinkMillis >= m_aniFrameMillis) {
    m_aniI++;
    bAnyChanged = true;
    m_aniBlinkMillis = currentMillis;
  }
  
  if (bAnyChanged) {
    byte *data;
    m_aniFrameMillis = applyPattern(&data, m_aniI);
    memcpy(this->RawData(), data, acGeometry::Width * acGeometry::Height / 8);
  }

  m_aniMillis = currentMillis;
  return true;
}

unsigned long acBitmap::applyPattern(byte **data, int aniIdx) {  
  switch(m_aniMode)
  {
    default:
    case 0:  // Space Invaders
      *data = (byte*)SI[aniIdx % 2];
      return 125;

    case 1:  // Nyan Cat
      m_aniFrameMillis = 200;
      *data = (byte*)NC[aniIdx % 2];
      return 200;

    case 2: 
      *data = (byte*)ANI;
      applyRainPattern(*data, aniIdx);
      return 70;

    case 3: 
      *data = (byte*)ANI;
      applyWavePattern(*data, aniIdx);
      return 100;

    case 4: 
      *data = (byte*)ANI;
      dopplerPattern(*data, aniIdx);
      return 80;

    case 5: 
      *data = (byte*)ANI;
      m_aniFrameMillis = 40;
      applyRandomDots(*data, aniIdx);
      return 40;

    case 6: 
      *data = (byte*)ANI;
      circlePattern(*data, aniIdx);
      return 105;
  }
}

void acBitmap::setPixel(byte *data, int x, int y, bool val) {
  int b = 8 - 1 - (x % 8);
  int xx = x / 8;
  int i = xx + (acGeometry::Height - 1 - y) * acGeometry::ElementsNX;
  bitWrite(data[i], b, val ? (byte)1 : (byte)0);
}

const int rdNum = 12;
int rdPos[][rdNum] = { 0 };
void acBitmap::applyRainPattern(byte *data, int aniIdx) {
  if (aniIdx < 1) {
    for(int rd = 0; rd < rdNum; ++rd) {
      rdPos[0][rd] = random(0, acGeometry::Width);
      rdPos[1][rd] = random(0, acGeometry::Height);
    }
  } else {
    for(int rd = 0; rd < rdNum; ++rd) {
      if (rdPos[1][rd] > 0) {
        rdPos[1][rd]--;
      } else {
        rdPos[0][rd] = random(0, acGeometry::Width);
        rdPos[1][rd] = acGeometry::Height - 1;
      }
    }
  }

  for (int y = 0; y < acGeometry::Height; y++) {
    for (int x = 0; x < acGeometry::Width; x++) {
      bool hit = false;
      for(int rd = 0; rd < rdNum; ++rd) {
        if (rdPos[0][rd] == x && rdPos[1][rd] == y) {
          hit = true;
          break;
        }
      }
      setPixel(data, x, y, hit);
    }
  }
}

void acBitmap::applyRandomDots(byte *data, int aniIdx) {
  clear(data);

  int numDots = random(5, 30);
  for (int i = 0; i < numDots; i++) {
    int x = random(0, acGeometry::Width);
    int y = random(0, acGeometry::Height);
      setPixel(data, x, y, true);
  }
}

void acBitmap::applyWavePattern(byte *data, int aniIdx) {
  for (int x = 0; x < acGeometry::Width; x++) {

    float fRad = (((double)x + aniIdx + 3) / (double)acGeometry::Width) * 1.6 * 3.141;
    double fSin = 0.5 * (sin(fRad) + 1.0);
    int nThrshld = (int)(fSin * (double)acGeometry::Height);

    for (int y = 0; y < acGeometry::Height; y++) {      
        setPixel(data, x, y, y <= nThrshld);
    }
  }
}

void acBitmap::dopplerPattern(byte *data, int aniIdx) {
  clear(data);

  int nHW = acGeometry::Width / 2;
  for (int x = 0; x < nHW; x++) {

    // Sinc(x)=sin(πx)/(πx) 
    double pi = 3.141;
    float fRad = ((double)x / (double)nHW) * (double)aniIdx * 0.35 * pi;
    double fSin = sin(fRad) / (fRad);
    bool val = fSin >= 0.0;

    for (int y = 2; y < acGeometry::Height - 2; y++) {
        setPixel(data, x,                         y, val); 
        setPixel(data, acGeometry::Width - 1 - x, y, val);
    }
  }
}

void acBitmap::circlePattern(byte *data, int aniIdx) {
  clear(data);

  int cY = acGeometry::Height / 2;
  int cX0 = acGeometry::Width / 3;
  int cX1 = 2 * acGeometry::Width / 3;

  int rL = acGeometry::Width / 3;
  int rR = aniIdx % (2 * rL);
  if (rR > rL)
    rR = (2 * rL) - rR;

  for (int y = 0; y < acGeometry::Height; y++) {
    for (int x = 0; x < acGeometry::Width; x++) {
      int x0 = x - cX0;
      int x1 = x - cX1;
      int y0 = y - cY;
      double r0 = sqrt(x0 * x0 + y0 * y0);
      double r1 = sqrt(x1 * x1 + y0 * y0);
      bool hit = (r0 >= rR - 0.5 && r0 < rR + 0.5) || (r1 >= rR - 0.5 && r1 < rR + 0.5); 
      setPixel(data, x, y, hit);
    }
  }
}
