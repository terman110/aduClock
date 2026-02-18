#ifndef __AC_BITMAP_H__
#define __AC_BITMAP_H__

#include "acGeometry.h"
#include "acCharSet.h"
#include "acTime.h"
#include <Arduino.h>
#include <EEPROM.h>

class acBitmap
{
protected:
    byte m_data[acGeometry::Width * acGeometry::Height / 8] = {0};
    acCharSet m_char;
    bool m_blink = false;
    bool m_blinkState = false;

    bool m_animate = false;
    unsigned long m_aniStartMilli = millis();
    int m_aniMode = 0;
    int m_aniI = 0;
    unsigned long m_aniFrameMillis = 250;
    unsigned long m_aniMillis = 0;
    unsigned long m_aniBlinkMillis = 0;

    const int m_aniNum = 5;

public:  
    // Constructor
    acBitmap();
    
    // Are dots blinking?
    bool GetBlink();

    // Blink dots?
    void SetBlink(bool _blink);

    // Get raw data
    byte* RawData() { return (byte*)m_data; }

    // Used character set
    acCharSet* CharSet() { return &m_char; }

    //  Generate (blinking) dot in bitmap buffer
    void dotGenerator();

    //  Generate four clock digets to bitmap buffer
    void clockGenerator(byte h, byte m);
    
    //  Clear bitmap buffer
    void clear();
    void clear(byte *data);

    void SetAnimate(bool animate) { m_animate = animate; }
    bool GetAnimate() { return m_animate; }
    bool DoAnimate(acTime *tm, unsigned long currentMillis, bool* updateBitmap);

    void setPixel(byte *data, int x, int y, bool val);
    unsigned long applyPattern(byte **data, int aniIdx);
    void applyRainPattern(byte *data, int aniIdx);
    void applyRandomDots(byte *data, int aniIdx);
    void applyWavePattern(byte *data, int aniIdx);
    void dopplerPattern(byte *data, int aniIdx);
    void circlePattern(byte *data, int aniIdx);
};

#endif // __AC_BITMAP_H__
