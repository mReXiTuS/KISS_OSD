#ifndef MYMAX7456_H
#define MYMAX7456_H

#include "MAX7456.h"
#include "CSettings.h"

class CMyMax7456 : public MAX7456
{
  public:
    CMyMax7456(uint8_t chipSelect);
    uint8_t printInt16(volatile uint8_t col, uint8_t row, int16_t value, uint8_t dec, uint8_t AlignLeft, const char* suffix = "", uint8_t blanks = 0, _OSDItemPos item = (_OSDItemPos)0, const char* prefix = "");
    void printP(uint8_t col, uint8_t row, const char *key, uint8_t menuItem = 200);
    void printInt16P(uint8_t col, uint8_t row, const char *key, int16_t value, uint8_t dec, uint8_t AlignLeft, const char* suffix = "", uint8_t blanks = 0);
    void printIntArrow(uint8_t col, uint8_t row, const char *key, int16_t value, uint8_t dec, uint8_t AlignLeft, uint8_t menuItem, const char* suffix = "", uint8_t blanks = 0);
    void printTime(volatile uint8_t col, uint8_t row, unsigned long time, const char* prefix = "", _OSDItemPos item = (_OSDItemPos)CSettings::OSD_ITEMS_POS_SIZE);
    void blink1sec();
    void printSpaces(uint8_t printLength);
    uint8_t checkPrintLength(volatile uint8_t col, uint8_t row, uint8_t printLength, uint8_t &blanks, _OSDItemPos item);
    uint8_t topOffset;
  protected:
    void printInt16(uint8_t col, uint8_t row, char *key, int16_t value, uint8_t dec, uint8_t AlignLeft, const char* suffix = "", uint8_t blanks = 0);
    uint8_t print_int16(int16_t p_int, char *str, uint8_t dec, uint8_t AlignLeft);
    void print_time(unsigned long time, char *time_str);
    void printInternal(const char* suffix, uint8_t blanks = 0);
    void checkArrow(uint8_t currentRow, uint8_t menuItem);
    boolean blinkActive;
};

#endif
