#include <Arduino.h>

#include "acCharSet.h"

#include <EEPROM.h>

const byte NUM[3][10][4] = 
{
  // 7 Segment
  {
    // 0
    { 
      B11111001,
      B10011001,
      B10011001,
      B10011111
    }
    ,
    // 1
    { 
      B00010001,
      B00010001,
      B00010001,
      B00010001
    }
    ,
    // 2
    { 
      B11110001,
      B00010001,
      B11111000,
      B10001111
    }
    ,
    // 3
    { 
      B11110001,
      B00010001,
      B01110001,
      B00011111
    }
    ,
    // 4
    { 
      B10011001,
      B10011001,
      B11110001,
      B00010001
    }
    ,
    // 5
    { 
      B11111000,
      B10001000,
      B11110001,
      B00011111
    }
    ,
    // 6
    { 
      B11111000,
      B10001000,
      B11111001,
      B10011111
    }
    ,
    // 7
    { 
      B11110001,
      B00010001,
      B00010001,
      B00010001
    }
    ,
    // 8
    { 
      B11111001,
      B10011001,
      B11111001,
      B10011111
    }
    ,
    // 9
    { 
      B11111001,
      B10011111,
      B00010001,
      B00011111
     }
  }
  ,
  {
  // Cursive
    // 0
    { 
      B01101001,
      B10011001,
      B10011001,
      B10010110
    }
    ,
    // 1
    { 
      B00010011,
      B01010001,
      B00010001,
      B00010001
    }
    ,
    // 2
    { 
      B01101001,
      B00010001,
      B00100100,
      B10001111
    }
    ,
    // 3
    { 
      B01101001,
      B00010001,
      B01100001,
      B10010110
    }
    ,
    // 4
    { 
      B10001000,
      B10101010,
      B11110010,
      B00100010
    }
    ,
    // 5
    { 
      B11111000,
      B10001000,
      B01100001,
      B10010110
    }
    ,
    // 6
    { 
      B01101001,
      B10001000,
      B11101001,
      B10010110
    }
    ,
    // 7
    { 
      B11110001,
      B00010010,
      B01000100,
      B01000100
    }
    ,
    // 8
    { 
      B01101001,
      B10011001,
      B01101001,
      B10010110
    }
    ,
    // 9
    { 
      B01101001,
      B10010111,
      B00010001,
      B10010110
    }
  }
  ,
  {
  // Binary
    // 0
    { 
      B00000000,
      B00000000,
      B00000000,
      B00000000,
    }
    ,
    // 1
    { 
      B00000000,
      B00000000,
      B00000000,
      B01100000,
    }
    ,
    // 2
    { 
      B00000000,
      B00000000,
      B01100000,
      B00000000,
    }
    ,
    // 3
    { 
      B00000000,
      B00000000,
      B01100000,
      B01100000,
    }
    ,
    // 4
    { 
      B00000000,
      B01100000,
      B00000000,
      B00000000,
    }
    ,
    // 5
    { 
      B00000000,
      B01100000,
      B00000000,
      B01100000,
    }
    ,
    // 6
    { 
      B00000000,
      B01100000,
      B01100000,
      B00000000,
    }
    ,
    // 7
    { 
      B00000000,
      B01100000,
      B01100000,
      B01100000,
    }
    ,
    // 8
    { 
      B01100000,
      B00000000,
      B00000000,
      B00000000,
    }
    ,
    // 9
    { 
      B01100000,
      B00000000,
      B00000000,
      B01100000,
    }
  }
};


const byte NUM_H[8] = 
{
  B01000010,
  B01000010,
  B01000010,
  B01000010,
  B01111110,
  B01000010,
  B01000010,
  B01000010
};

const byte NUM_M[8] = 
{
  B01000010,
  B01100110,
  B01011010,
  B01000010,
  B01000010,
  B01000010,
  B01000010,
  B01000010
};

const byte NUM_B[8] = 
{
  B01111100,
  B01000110,
  B01000010,
  B01111100,
  B01000010,
  B01000010,
  B01000110,
  B01111100
};

acCharSet::acCharSet()
{
  ReadFromEPROM();
}

acCharSet::acCharSet(enCharSet selectedSet)
{
  CurrentSet = selectedSet;
}

int acCharSet::next()
{
  int cur = (int)CurrentSet + 1;
  if (cur >= Count)
    cur = (enCharSet)0;
  CurrentSet = (enCharSet)cur;
  return cur;
}

void acCharSet::ReadFromEPROM()
{
  int chartmp = EEPROM.read(1);
  if ( chartmp >= 0 && chartmp < (int)Count)
    CurrentSet = (enCharSet)chartmp;
}

void acCharSet::WriteToEPROM()
{
  EEPROM.write( 1, (int)CurrentSet);
}

byte acCharSet::get(int n, int y)
{
  if (n >= 0 && n < 10)
    return get((char)(n + 48), y);
  return get(' ', y);
}

byte acCharSet::get(char c, int y)
{
  switch(c)
  {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return (y < 0 || y >= 4) ? 0 : NUM[(int)CurrentSet][(int)c - 48][y];
    case 'h':
    case 'H':
      return (y < 0 || y >= 8) ? 0 : NUM_H[y];
    case 'm':
    case 'M':
      return (y < 0 || y >= 8) ? 0 : NUM_M[y];
    case 'b':
    case 'B':
      return (y < 0 || y >= 8) ? 0 : NUM_B[y];
    default:
      return (y < 0 || y >= 4) ? 0 : NUM[(int)Binary][0][y];
      
  }
}
