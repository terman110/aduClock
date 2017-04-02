#include <Arduino.h>

#include "acCharSet.h"

#include <EEPROM.h>

// Alpha numeric 3x3 font
// https://tonypai.itch.io/3x3-pixel-font
const bool ALPHNUM_3X3[56][9] =
{
  // A
  {
    0, 1, 0,
    1, 1, 1,
    1, 0, 1
  },
  // B
  {
    1, 1, 0,
    1, 1, 1,
    1, 1, 1
  },
  // C
  {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1
  },
  // D
  {
    1, 1, 0,
    1, 0, 1,
    1, 1, 0
  },
  // E
  {
    1, 1, 1,
    1, 1, 0,
    1, 1, 1
  },
  // F
  {
    1, 1, 1,
    1, 1, 0,
    1, 0, 0
  },
  // G
  {
    1, 1, 0,
    1, 0, 1,
    1, 1, 1
  },
  // H
  {
    1, 0, 1,
    1, 1, 1,
    1, 0, 1
  },
  // I
  {
    1, 1, 1,
    0, 1, 0,
    1, 1, 1
  },
  // J
  {
    0, 0, 1,
    1, 0, 1,
    1, 1, 1
  },
  // K
  {
    1, 0, 1,
    1, 1, 0,
    1, 0, 1
  },
  // L
  {
    1, 0, 0,
    1, 0, 0,
    1, 1, 1
  },
  // M
  {
    1, 1, 1,
    1, 1, 1,
    1, 0, 1
  },
  // N
  {
    1, 1, 1,
    1, 0, 1,
    1, 0, 1
  },
  // O
  {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
  },
  // P
  {
    1, 1, 1,
    1, 1, 1,
    1, 0, 0
  },
  // Q
  {
    1, 1, 1,
    1, 1, 1,
    0, 0, 1
  },
  // R
  {
    1, 1, 1,
    1, 0, 0,
    1, 0, 0
  },
  // S
  {
    0, 1, 1,
    0, 1, 0,
    1, 1, 0
  },
  // T
  {
    1, 1, 1,
    0, 1, 0,
    0, 1, 0
  },
  // U
  {
    1, 0, 1,
    1, 0, 1,
    1, 1, 1
  },
  // V
  {
    1, 0, 1,
    1, 0, 1,
    0, 1, 0
  },
  // W
  {
    1, 0, 1,
    1, 1, 1,
    1, 1, 1
  },
  // X
  {
    1, 0, 1,
    0, 1, 0,
    1, 0, 1
  },
  // Y
  {
    1, 0, 1,
    0, 1, 0,
    0, 1, 0
  },
  // Z
  {
    1, 1, 0,
    0, 1, 0,
    0, 1, 1
  },
  // 0
  {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
  },
  // 1
  {
    1, 1, 0,
    0, 1, 0,
    1, 1, 1
  },
  // 2
  {
    1, 1, 0,
    0, 1, 0,
    0, 1, 1
  },
  // 3
  {
    1, 1, 1,
    0, 1, 1,
    1, 1, 1
  },
  // 4
  {
    1, 0, 1,
    1, 1, 1,
    0, 0, 1
  },
  // 5
  {
    0, 1, 1,
    0, 1, 0,
    1, 1, 0
  },
  // 6
  {
    1, 0, 0,
    1, 1, 1,
    1, 1, 1
  },
  // 7
  {
    1, 1, 1,
    0, 0, 1,
    0, 0, 1
  },
  // 8
  {
    0, 1, 1,
    1, 1, 1,
    1, 1, 1
  },
  // 9
  {
    1, 1, 1,
    1, 1, 1,
    0, 0, 1
  },
  // (
  {
    0, 0, 1,
    0, 1, 0,
    0, 0, 1
  },
  // )
  {
    1, 0, 0,
    0, 1, 0,
    1, 0, 0
  },
  // <
  {
    0, 1, 1,
    1, 1, 0,
    0, 1, 1
  },
  // >
  {
    1, 1, 0,
    0, 1, 1,
    1, 1, 0
  },
  // [
  {
    0, 1, 1,
    0, 1, 0,
    0, 1, 1
  },
  // [
  {
    1, 1, 0,
    0, 1, 0,
    1, 1, 0
  },
  // back-slash
  {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  },
  // /
  {
    0, 0, 1,
    0, 1, 0,
    1, 0, 0
  },
  // |
  {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
  },
  // _
  {
    0, 0, 0,
    0, 0, 0,
    1, 1, 1
  },
  // -
  {
    0, 0, 0,
    1, 1, 1,
    0, 0, 0
  },
  // .
  {
    0, 0, 0,
    0, 0, 0,
    0, 1, 0
  },
  // :
  {
    0, 1, 0,
    0, 0, 0,
    0, 1, 0
  },
  // +
  {
    0, 1, 0,
    1, 1, 1,
    0, 1, 0
  },
  // =
  {
    1, 1, 1,
    0, 0, 0,
    1, 1, 1
  },
  // ?
  {
    1, 1, 1,
    0, 1, 1,
    0, 1, 0
  },
  // '
  {
    0, 1, 0,
    0, 0, 0,
    0, 0, 0
  },
  // "
  {
    1, 0, 1,
    0, 0, 0,
    0, 0, 0
  },
  // ,
  {
    0, 0, 0,
    0, 1, 0,
    1, 0, 0
  },
  // ^
  {
    0, 1, 0,
    1, 0, 1,
    0, 0, 0
  }
};

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
  switch (c)
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

const bool* acCharSet::get_3x3(int n)
{
  if (n >= 0 && n < 10)
    return (const bool*)get_3x3((char)(n + 48));
  return (const bool*)get_3x3(' ');
}

const bool* acCharSet::get_3x3(char c)
{
  int nVal = (int)c;
  if(nVal >= 48 && nVal <= 57)  // 0 ... 9 => 26 ... 35
    return ALPHNUM_3X3[nVal - 22];
  if(nVal >= 65 && nVal <= 90)  // A ... Z => 0 ... 25
    return ALPHNUM_3X3[nVal - 65];
  if(nVal >= 97 && nVal <= 122)  // a ... Z => 0 ... 25
    return ALPHNUM_3X3[nVal - 97];
  switch (c)
  {
    case '"': return ALPHNUM_3X3[62];
    case '\'': return ALPHNUM_3X3[61];
    case '/': return ALPHNUM_3X3[43];
    case '|': return ALPHNUM_3X3[44];
    case '(': return ALPHNUM_3X3[36];
    case ')': return ALPHNUM_3X3[37];
    case '<': return ALPHNUM_3X3[38];
    case '>': return ALPHNUM_3X3[39];
    case '{': return ALPHNUM_3X3[38];
    case '}': return ALPHNUM_3X3[39];
    case '[': return ALPHNUM_3X3[40];
    case ']': return ALPHNUM_3X3[41];
    case '\\': return ALPHNUM_3X3[42];
    case '-': return ALPHNUM_3X3[46];
    case '_': return ALPHNUM_3X3[45];
    case '.': return ALPHNUM_3X3[47];
    case ',': return ALPHNUM_3X3[63];
    case ';': return ALPHNUM_3X3[48];
    case ':': return ALPHNUM_3X3[48];
    case '?': return ALPHNUM_3X3[60];
    case '=': return ALPHNUM_3X3[50];
    case '+': return ALPHNUM_3X3[49];
    case '^': return ALPHNUM_3X3[64];
    default:
      return (const bool*)new bool[9] { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  }
}

