#ifndef COMMON_H
#define COMMON_H

#include "MyArduboy.h"

/*  Defines  */

//#define DEBUG
#define APP_TITLE       "HOPPER"
#define APP_INFO        "OBN-Y02 VER 0.21"
#define APP_RELEASED    "NOVEMBER 2017"

/*  Typedefs  */

typedef unsigned char   uchar;
typedef unsigned int    uint;

/*  Global Functions  */

void initLogo(void);
bool updateLogo(void);
void drawLogo(void);

void initTitle(void);
bool updateTitle(void);
void drawTitle(void);
uint8_t setLastScore(int score, uint32_t time);
void enterHighScore(byte file);



void initGame(void);
bool updateGame(void);
void drawGame(void);

/*  Global Variables  */

extern MyArduboy arduboy;

/*  For Debugging  */

#ifdef DEBUG
extern bool             dbgPrintEnabled;
extern char             dbgRecvChar;
#define dprint(...)     (!dbgPrintEnabled || Serial.print(__VA_ARGS__))
#define dprintln(...)   (!dbgPrintEnabled || Serial.println(__VA_ARGS__))
#else
#define dprint(...)
#define dprintln(...)
#endif

#endif // COMMON_H
