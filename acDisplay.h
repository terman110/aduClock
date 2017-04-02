#ifndef __AC_DISPLAY_H__
#define __AC_DISPLAY_H__

#include "acGeometry.h"
#include "acBitmap.h"

#include <Arduino.h>

class acDisplay
{
protected:
    // Shift register latch pin 
    int m_latchPin;
    // Shift register clock pin
    int m_clockPin;
    // Shift register data pin
    int m_dataPin;
    //  Output row index
    int m_cRow = 0;
    
public:
    // Constructor
    acDisplay(int latchPin = 2, int clockPin = 3, int dataPin = 4);

    //  Bit bang bitmap buffer to LED matrix
    void printBitmap(acBitmap bitmap);

    // Clear display
    void clearDisplay();

    // Next row to be printed is row == 0
    bool StartingPrint() { return m_cRow == 0; }
    
    // Reverse byte order
    static byte rb( byte x);
};

#endif // __AC_DISPLAY_H__
