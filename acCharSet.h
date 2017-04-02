#ifndef __NUM_CHAR_H__
#define __NUM_CHAR_H__

enum enCharSet
{
  Segment7 = 0,
  Cursive = 1,
  Binary = 2
};
    
class acCharSet
{
  public:
    // Number of character sets
    static const int Count = 3;
    //  Number width
    static const int NumberWidth = 4;
    //  Character width
    static const int CharWidth = 8;
    // Selected character sets
    enCharSet CurrentSet;

    // Constructor: Load char set index from EEPROM
    acCharSet();

    // Constructor: Set char set
    acCharSet(enCharSet selectedSet);

    // Select next char set
    int next();

    // Read char set index from EEPROM
    void ReadFromEPROM();

    // Write char set index to EEPROM
    void WriteToEPROM();

    // Get byte
    byte get(int n, int y);

    // Get byte
    byte get(char c, int y);

    // Get 3x3 font bool array
    const bool* get_3x3(int n);
    
    // Get 3x3 font bool array
    const bool* get_3x3(char c);
};
#endif // __NUM_CHAR_H__
