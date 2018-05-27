#include "MyMax7456.h"
#include "fixFont.h"
#include "CSettings.h"
#include "Config.h"

extern boolean timer1sec;

//#define COMPRESSED_FONT

extern CSettings settings;

CMyMax7456::CMyMax7456(uint8_t chipSelect) : MAX7456(chipSelect), blinkActive(false), topOffset(3)
{
}

void CMyMax7456::blink1sec()
{
  blinkActive = true;
}

void CMyMax7456::printSpaces(uint8_t col, uint8_t row, uint8_t printLength)
{
  setCursor(col, row);
  printSpaces(printLength);
}

void CMyMax7456::printSpaces(uint8_t printLength)
{
  uint8_t i;  
  for(i=0; i<printLength; i++) 
  {
    print(fixChar(' '));  
  }
}

uint8_t CMyMax7456::checkPrintLength(volatile uint8_t col, uint8_t row, uint8_t printLength, uint8_t &blanks, _OSDItemPos item)
{
  uint8_t lengthCorrection = 0;
  if(col+printLength > (settings.COLS-settings.s.m_goggle))
  {
    //col = COLS-printLength;
    lengthCorrection = settings.COLS-printLength-settings.s.m_goggle;
    printSpaces(lengthCorrection-blanks, row, blanks);
    blanks = 0;
    if(item < CSettings::OSD_ITEMS_POS_SIZE) settings.m_colBorder[item] = true;
  }
  else
  {
    setCursor(col, row);
    if(item < CSettings::OSD_ITEMS_POS_SIZE) settings.m_colBorder[item] = false;
    if(item < CSettings::OSD_ITEMS_POS_SIZE && ((col+printLength) == (settings.COLS-settings.s.m_goggle) || (col+printLength) == (settings.COLS-settings.s.m_goggle-1))) settings.m_colBorder[item] = true;
    if((col+printLength) == (settings.COLS-settings.s.m_goggle))
    {
      printSpaces(col-blanks, row, blanks);
      blanks = 0;      
    }
  }
  return lengthCorrection;
}

static char printBuf2[30];

uint8_t CMyMax7456::printInt16(volatile uint8_t col, uint8_t row, int16_t value, uint8_t dec, const char* suffix, uint8_t blanks = 0, _OSDItemPos item, const char* prefix)
{
  print_int16(value, printBuf2, dec);
  uint8_t printLength = strlen(printBuf2)+strlen(prefix)+strlen(suffix);
  uint8_t lengthCorrection = checkPrintLength(col, row, printLength, blanks,item);
  if(blinkActive && timer1sec)
  {
    printSpaces(printLength);
  }
  else
  {
    print(fixStr(prefix));
    printInternal(suffix, blanks);
  }
  blinkActive = false;
  return lengthCorrection;
}

void CMyMax7456::printP(uint8_t col, uint8_t row, const char *key, uint8_t menuItem)
{
  uint8_t printLength = strlen_P(key);
  uint8_t blanks = 0;
  checkPrintLength(col, row, printLength, blanks, CSettings::OSD_ITEMS_POS_SIZE);
  if(blinkActive && timer1sec)
  {
    if(menuItem < 200) printLength++;
    printSpaces(printLength);
  }
  else
  {
    if(menuItem < 200)
    {
      checkArrow(row, menuItem);
    }
    print(fixPStr(key));
  }
  blinkActive = false;
}

void CMyMax7456::printInt16(uint8_t col, uint8_t row, char *key, int16_t value, uint8_t dec, const char* suffix, uint8_t blanks)
{
  print_int16(value, printBuf2, dec);
  uint8_t printLength = strlen(printBuf2)+strlen(suffix)+strlen(key)+blanks;
  checkPrintLength(col, row, printLength, blanks, CSettings::OSD_ITEMS_POS_SIZE);
  if(blinkActive && timer1sec)
  {
    printSpaces(printLength);
  }
  else
  {
    print(fixStr(key));
    printInternal(suffix, blanks);
  }
  blinkActive = false;
}

void CMyMax7456::printInt16P(uint8_t col, uint8_t row, const char *key, int16_t value, uint8_t dec, const char* suffix, uint8_t blanks)
{
  printInt16(col, row, fixPStr(key), value, dec, suffix, blanks);
}

void CMyMax7456::printIntArrow(uint8_t col, uint8_t row, const char *key, int16_t value, uint8_t dec, uint8_t menuItem, const char* suffix, uint8_t blanks)
{
  setCursor(col, row);
  checkArrow(row, menuItem); 
  printInt16(col+1, row, fixPStr(key), value, dec, suffix, blanks);
}

void CMyMax7456::checkArrow(uint8_t currentRow, uint8_t menuItem)
{
  if(currentRow-topOffset == menuItem)
  {
    print(fixChar('>'));
  }
  else
  {
    print(fixChar(' '));
  }
}

void CMyMax7456::printTime(volatile uint8_t col, uint8_t row, unsigned long time, const char* prefix, _OSDItemPos item)
{
  print_time(time, printBuf2);
  uint8_t printLength = strlen(printBuf2)+strlen(prefix);
  uint8_t blanks = 0;
  checkPrintLength(col, row, printLength, blanks, item);
  if(blinkActive && timer1sec)
  {
    printSpaces(printLength);
  }
  else
  {
    print(prefix);
    print(printBuf2);
  }
  blinkActive = false;
}

void CMyMax7456::printInternal(const char* suffix, uint8_t blanks)
{
  print(printBuf2);
  print(fixStr(suffix));
  printSpaces(blanks);
}

uint8_t CMyMax7456::print_int16(int16_t p_int, char *str, uint8_t dec)
{
  int16_t useVal = p_int;
  uint8_t aciidig[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
  uint8_t i = 0;
  int8_t digits[5] = {-1,-1,-1,-1,-1};
  uint8_t CharPos = 0;
  if(p_int < 0){
      useVal = p_int*-1;
      str[CharPos] = '-';
      CharPos++;
  }
  while(useVal >= 10000){digits[0]++; useVal-=10000;}
  while(useVal >= 1000){digits[1]++; useVal-=1000;}
  while(useVal >= 100){digits[2]++; useVal-=100;}
  while(useVal >= 10){digits[3]++; useVal-=10;}
  digits[4] = useVal-1;
  if(digits[4] < -1) digits[4] = -1;
  //char result[6] = {' ',' ',' ',' ',' ','0'};
  bool started = false;
  for(i = 0; i < 5;i++)
  {
    if(i == 5-dec && dec > 0)
    {
      if(CharPos == 0 || (CharPos == 1 && p_int < 0))
      {
        str[CharPos] = fixChar('0');
        CharPos++;
      }
      str[CharPos] = fixChar('.');
      CharPos++;
      started = true;
    }
    if(started || digits[i] > -1 || (dec > 0 && 6-dec == i))
    {
      str[CharPos] = fixNo(aciidig[digits[i]+1]);
      CharPos++;
      started = true;
    }
  }
  if(CharPos == 0)
  {
    str[CharPos] = fixChar('0');
    CharPos++;
  }
  str[CharPos] = 0x00;
  return CharPos;
}

void CMyMax7456::print_time(unsigned long time, char *time_str) 
{
    uint16_t seconds = time / 1000;
    uint8_t minutes = seconds / 60;
    if (seconds >= 60) 
    {
      minutes = seconds/60;
    } 
    else 
    {
      minutes = 0;
    }
    seconds = seconds - (minutes * 60);
    static char time_sec[6];
    uint8_t i = 0;
    uint8_t time_pos = print_int16(minutes, time_str,0);
    time_str[time_pos++] = fixChar(':');
    
    uint8_t sec_pos = print_int16(seconds, time_sec,0);
    if(seconds < 10) 
    {
      time_str[time_pos++] = fixChar('0');
    }
    for (i=0; i<sec_pos; i++)
    {
      time_str[time_pos++] = time_sec[i];
    }
    time_str[time_pos++] = 0x00;
}

