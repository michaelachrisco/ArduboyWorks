#pragma once

#include "MyArduboy2.h"

/*  Defines  */

//#define DEBUG
#define FPS             60
#define APP_TITLE       "EVADE"
#define APP_CODE        "OBN-Y14" // TODO: Modify
#define APP_VERSION     "0.01"
#define APP_RELEASED    "AUGUST 2020"

enum MODE_T : uint8_t {
    MODE_LOGO = 0,
    MODE_TITLE,
    MODE_GAME,
};

/*  Typedefs  */

typedef struct {
    uint32_t    dummy;
    uint32_t    playFrames;
    uint16_t    playCount;
} RECORD_T; // sizeof(RECORD_T) is 10 bytes

/*  Global Functions (Common)  */

void    readRecord(void);
void    writeRecord(void);
void    clearRecord(void);
void    handleDPad(void);
void    drawTime(int16_t x, int16_t y, uint32_t frames);

void    setSound(bool on);
void    playSoundTick(void);
void    playSoundClick(void);

void    eepSeek(int addr);
uint8_t eepRead8(void);
uint16_t eepRead16(void);
uint32_t eepRead32(void);
void    eepReadBlock(void *p, size_t n);
void    eepWrite8(uint8_t val);
void    eepWrite16(uint16_t val);
void    eepWrite32(uint32_t val);
void    eepWriteBlock(const void *p, size_t n);

/*  Global Functions (Menu)  */

void    clearMenuItems(void);
void    addMenuItem(const __FlashStringHelper *label, void (*func)(void));
int8_t  getMenuItemPos(void);
int8_t  getMenuItemCount(void);
void    setMenuCoords(int8_t x, int8_t y, int8_t w, int8_t h, bool f, bool s);
void    setMenuItemPos(int8_t pos);
void    setConfirmMenu(int8_t y, void (*funcOk)(), void (*funcCancel)());
void    handleMenu(void);
void    drawMenuItems(bool isForced);
void    drawSoundEnabled(void);

/*  Global Functions (Each Mode)  */

void    initLogo(void);
MODE_T  updateLogo(void);
void    drawLogo(void);

void    initTitle(void);
MODE_T  updateTitle(void);
void    drawTitle(void);

void    initGame(void);
MODE_T  updateGame(void);
void    drawGame(void);

/*  Global Variables  */

extern MyArduboy2   arduboy;
extern RECORD_T     record;
extern uint8_t      counter;
extern int8_t       padX, padY, padRepeatCount;
extern uint16_t     lastScore;
extern bool         isInvalid, isRecordDirty;

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
