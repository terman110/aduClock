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
    unsigned long m_aniMillis = 0;
    unsigned long m_aniBlinkMillis = 0;

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

    // Generate menu entry to bitmap buffer
    void menuGenerator(int menu, int val);
    
    //  Clear bitmap buffer
    void clear();

    void SetAnimate(bool animate) { m_animate = animate; }
    bool GetAnimate() { return m_animate; }
    bool DoAnimate(acTime *tm, unsigned long currentMillis, bool* updateBitmap);
};

#endif // __AC_BITMAP_H__
