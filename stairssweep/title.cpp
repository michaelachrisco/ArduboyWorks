#include "common.h"

/*  Defines  */

#define IMG_TITLE_W 64
#define IMG_TITLE_H 64

enum STATE_T {
    STATE_INIT = 0,
    STATE_TITLE,
    STATE_RECORD,
    STATE_CREDIT,
    STATE_STARTED,
};

#define APP_TITLE_V     "STAIRS  \0   SWEEP"
#define APP_RELEASED_V  "OCTOBER\0" "2019"

/*  Typedefs  */

/*  Local Functions  */

static void onStart(void);
static void onRecord(void);
static void onCredit(void);
static void handleAnyButton(void);

static void drawTitleImage(void);
static void drawRecord(void);
static void drawCredit(void);
static void drawText(const char *p, int16_t x);

/*  Local Variables  */

PROGMEM static const uint8_t imgTitle[512] = { // 64x64
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0x7C, 0xB8, 0xD0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xC0, 0x80, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0x7C,
    0x3A, 0x17, 0x0F, 0x1F, 0x3F, 0x7D, 0xBE, 0xDF, 0xEF, 0xF7, 0xFB, 0x7F, 0x3F, 0x1F, 0x0E, 0x04,
    0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0x7C, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF1, 0xFB, 0xF5, 0xEE, 0xDF, 0xBE, 0x7D, 0xFB, 0xF7, 0xEF, 0xDF,
    0xBE, 0x7C, 0xF8, 0x70, 0x20, 0x00, 0x00, 0x01, 0x83, 0xC1, 0xE0, 0xF0, 0xF8, 0x7C, 0xFA, 0xF7,
    0xEF, 0xDF, 0xBF, 0x7D, 0xF8, 0xF0, 0xE0, 0xF1, 0xFB, 0x7F, 0x3F, 0x1F, 0x0E, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0xF8, 0xF0, 0xE0, 0xC0, 0xE0, 0xF0, 0xF8, 0x74,
    0x2E, 0x1F, 0x2F, 0x77, 0xFB, 0xFD, 0xFE, 0xDF, 0xEF, 0xF7, 0xFF, 0x7F, 0x3E, 0x1D, 0x0B, 0x07,
    0x83, 0xC1, 0xE0, 0xF0, 0xF8, 0x7C, 0xFA, 0xF7, 0xEF, 0xDF, 0xBF, 0x7D, 0xFE, 0xFF, 0xEF, 0xF7,
    0xFB, 0x7D, 0x3E, 0x1F, 0x0E, 0x05, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x1C, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0x7C, 0xBE, 0xDD, 0xEB, 0xF7, 0xEF, 0xDF, 0xBE, 0x7C,
    0xF8, 0xF0, 0xF8, 0x7C, 0x3E, 0x1D, 0x0B, 0x07, 0x83, 0x01, 0x00, 0x10, 0x38, 0x7C, 0xFA, 0xF7,
    0xEF, 0xDF, 0xBF, 0x7D, 0xFE, 0xFF, 0xEF, 0xF7, 0xFB, 0x7D, 0x3E, 0x1F, 0x0E, 0x05, 0x03, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x5F, 0xEF, 0xF7, 0xFB, 0x7D, 0x3F, 0x1F, 0x0F, 0x07,
    0x82, 0x41, 0xE0, 0xF0, 0xE0, 0xC0, 0x82, 0x07, 0x0F, 0x1F, 0x3E, 0x7C, 0xF8, 0xF0, 0xE0, 0xF1,
    0xFB, 0x7F, 0x3F, 0x1F, 0x0E, 0x05, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0xE0, 0xF0, 0xF8, 0xFC, 0xBE, 0xDF,
    0xEF, 0xF7, 0xFA, 0x7D, 0xFB, 0xF7, 0xEF, 0xDF, 0xBE, 0x7C, 0x3E, 0x1F, 0x0F, 0x07, 0x03, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x17,
    0x3B, 0x7D, 0x3E, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

PROGMEM static const char creditText[] = APP_TITLE_V "\0 \0" APP_RELEASED_V "\0\0" \
        "PROGRAMMED\0BY OBONO\0\0THIS\0PROGRAM IS\0RELEASED\0UNDER THE\0MIT\0 LICENSE.\0\e";

static STATE_T  state = STATE_INIT;
static int8_t   lastChoice, maxChoice;

/*---------------------------------------------------------------------------*/
/*                              Main Functions                               */
/*---------------------------------------------------------------------------*/

void initTitle(void)
{
    if (state == STATE_INIT) {
        readRecord();
        lastScore = 0;
        lastChoice = 1;
    }
    maxChoice = 2;
    uint8_t  hiLevel = record.hiscore[0].level;
    uint32_t hiScore = record.hiscore[0].score;
    if (hiLevel >= HILEVEL3 || hiScore >= HISCORE3) maxChoice = 3;
    if (hiLevel >= HILEVEL4 || hiScore >= HISCORE4) maxChoice = 4;

    clearMenuItems();
    addMenuItem(F("BEGINNER"), onStart);
    addMenuItem(F("NOVICE"), onStart);
    addMenuItem(F("STANDARD"), onStart);
    if (maxChoice >= 3) addMenuItem(F("VETERAN"), onStart);
    addMenuItem(NULL, NULL);
    addMenuItem(F("RECORD"), onRecord);
    addMenuItem(F("CREDIT"), onCredit);
    setMenuCoords(80, 63, 37, 64, false, true);
    setMenuItemPos(lastChoice);

    state = STATE_TITLE;
    isInvalid = true;
}

MODE_T updateTitle(void)
{
    MODE_T ret = MODE_TITLE;
    if (state == STATE_TITLE) {
        handleMenu();
        if (state == STATE_STARTED) {
            ret = MODE_GAME;
        }
    } else {
        handleAnyButton();
    }
    randomSeed(rand() ^ micros()); // Shuffle random
    return ret;
}

void drawTitle(void)
{
    if (isInvalid) {
        arduboy.clear();
        switch (state) {
        case STATE_TITLE:
        case STATE_STARTED:
            drawTitleImage();
            break;
        case STATE_RECORD:
            drawRecord();
            break;
        case STATE_CREDIT:
            drawCredit();
            break;
        default:
            break;
        }
    }
    if (state == STATE_TITLE) {
        drawMenuItems(isInvalid);
    }
    isInvalid = false;
}

/*---------------------------------------------------------------------------*/
/*                             Control Functions                             */
/*---------------------------------------------------------------------------*/

static void onStart(void)
{
    int8_t pos = getMenuItemPos();
    lastChoice = pos;
    if (pos == 3 && maxChoice == 4 && arduboy.buttonPressed(LEFT_BUTTON_V | RIGHT_BUTTON_V)) pos++;
    startLevel = (pos < 2) ? pos * 10 : (pos - 1) * 30; // { 0, 10, 30, 60, 90 }
    state = STATE_STARTED;
    isInvalid = true;
    dprint(F("Start Game "));
    dprintln(pos);
}

static void onRecord(void)
{
    playSoundClick();
    state = STATE_RECORD;
    isInvalid = true;
    dprintln(F("Show record"));
}

static void onCredit(void)
{
    playSoundClick();
    state = STATE_CREDIT;
    isInvalid = true;
    dprintln(F("Show credit"));
}

static void handleAnyButton(void)
{
    if (arduboy.buttonDown(A_BUTTON | B_BUTTON)) {
        playSoundClick();
        state = STATE_TITLE;
        isInvalid = true;
    }
}

/*---------------------------------------------------------------------------*/
/*                              Draw Functions                               */
/*---------------------------------------------------------------------------*/

static void drawTitleImage(void)
{
#ifdef DEBUG
    arduboy.printEx(66, 63, F("DEBUG"));
#endif
    arduboy.drawBitmap(8, 0, imgTitle, IMG_TITLE_W, IMG_TITLE_H, WHITE);
    if (lastScore > 0) drawNumberV(0, -1, lastScore, ALIGN_RIGHT);
}

static void drawRecord(void)
{
    arduboy.printEx(4, 55, F("BEST 5"));
    arduboy.printEx(10, 43, F("SCORES"));
    for (int i = 0; i < 5; i++) {
        arduboy.printEx(22 + i * 14, 61, F("[ ]"));
        drawNumberV(22 + i * 14, 55, i + 1, ALIGN_LEFT);
        drawNumberV(22 + i * 14, 1, record.hiscore[i].score, ALIGN_RIGHT);
        drawLabelLevel(28 + i * 14, 13);
        drawNumberV(28 + i * 14, -1, record.hiscore[i].level, ALIGN_RIGHT);
    }
    arduboy.drawRect(19, -1, 73, HEIGHT + 2, WHITE);
    arduboy.printEx(98, 63, F("PLAY COUNT"));
    drawNumberV(104, -1, record.playCount, ALIGN_RIGHT);
    arduboy.printEx(112, 63, F("PLAY TIME"));
    drawTime(118, -1, record.playFrames, ALIGN_RIGHT);
}

static void drawCredit(void)
{
    drawText(creditText, 24);
    arduboy.drawRect(21, 6, 17, 53, WHITE);
}

static void drawText(const char *p, int16_t x)
{
    while (pgm_read_byte(p) != '\e') {
        uint8_t len = strnlen_P(p, 21);
        arduboy.printEx(x, HEIGHT / 2 - 1 + len * 3, (const __FlashStringHelper *) p);
        p += len + 1;
        x += (len == 0) ? 2 : 6;
    }
}
