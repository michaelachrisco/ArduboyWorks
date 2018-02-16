#include "common.h"

/*  Defines  */

#define PIECES  10
#define BOARD_W 17
#define BOARD_H 9

#define EMPTY   -1
#define COLUMN  -2

enum {
    STATE_START = 0,
    STATE_GAME,
    STATE_OVER,
};

enum {
    ALIGN_LEFT = 0,
    ALIGN_CENTER,
    ALIGN_RIGHT,
};

/*  Typedefs  */

typedef struct {
    uint16_t    x : 5;
    uint16_t    y : 4;
    uint16_t    rot : 3;
    uint16_t    dummy : 4;
} PIECE_T;

/*  Local Functions  */

static bool putPieces(void);
static bool putPiece(int8_t idx);
static bool putPiecePart(int8_t idx, int8_t x, int8_t y, int8_t c);
static void drawBoard(void);
static void drawPieces(void);
static void drawPiece(int8_t idx);
static bool drawPiecePart(int8_t idx, int8_t x, int8_t y, int8_t c);
static bool execPiece(int8_t idx, bool (*f)(int8_t, int8_t, int8_t, int8_t));

/*  Local Variables  */

PROGMEM static const int8_t piecePtn[PIECES][16] = {
    { 0x5, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x2, 0x9, 0x9, 0x9,  0x9, 0xA, 0x0, 0x0 }, // white
    { 0x0, 0x0, 0x5, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x8, 0xB, 0x9,  0x9, 0xA, 0x0, 0x0 }, // green
    { 0x0, 0x5, 0x0, 0x0,  0x0, 0x5, 0x0, 0x0,  0x0, 0x2, 0x9, 0x9,  0x9, 0x1, 0x0, 0x0 }, // purple
    { 0x4, 0x9, 0xE, 0xA,  0x7, 0x0, 0x7, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }, // orange
    { 0x4, 0x9, 0x9, 0xA,  0x6, 0x0, 0x0, 0x0,  0x2, 0xA, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }, // red
    { 0x0, 0x0, 0x4, 0xA,  0x5, 0x0, 0x6, 0x0,  0x2, 0x9, 0x1, 0x0,  0x0, 0x0, 0x0, 0x0 }, // pink
    { 0x0, 0x8, 0xE, 0xA,  0x0, 0x0, 0x6, 0x0,  0x0, 0x8, 0x1, 0x0,  0x0, 0x0, 0x0, 0x0 }, // gray
    { 0x0, 0x0, 0x4, 0xA,  0x0, 0x0, 0x6, 0x0,  0x4, 0x9, 0x1, 0x0,  0x7, 0x0, 0x0, 0x0 }, // blue
    { 0x0, 0x5, 0x0, 0x0,  0x8, 0xF, 0xA, 0x0,  0x0, 0x7, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }, // brown
    { 0x0, 0x5, 0x0, 0x0,  0x0, 0xD, 0xA, 0x0,  0x0, 0x6, 0x0, 0x0,  0x8, 0x1, 0x0, 0x0 }, // yellow
};

PROGMEM static const int8_t pieceRot[8][16] = {
    { 0x0, 0x1, 0x2, 0x3,  0x4, 0x5, 0x6, 0x7,  0x8, 0x9, 0xA, 0xB,  0xC, 0xD, 0xE, 0xF },
    { 0x0, 0x2, 0x4, 0x1,  0x3, 0xA, 0x9, 0x8,  0x5, 0x6, 0x7, 0xD,  0xB, 0xE, 0xC, 0xF },
    { 0x0, 0x4, 0x3, 0x2,  0x1, 0x7, 0x6, 0x5,  0xA, 0x9, 0x8, 0xE,  0xD, 0xC, 0xB, 0xF },
    { 0x0, 0x3, 0x1, 0x4,  0x2, 0x8, 0x9, 0xA,  0x7, 0x6, 0x5, 0xC,  0xE, 0xB, 0xD, 0xF },
    { 0x0, 0x3, 0x4, 0x1,  0x2, 0x7, 0x6, 0x5,  0x8, 0x9, 0xA, 0xE,  0xC, 0xD, 0xB, 0xF },
    { 0x0, 0x1, 0x3, 0x2,  0x4, 0x8, 0x9, 0xA,  0x5, 0x6, 0x7, 0xC,  0xB, 0xE, 0xD, 0xF },
    { 0x0, 0x2, 0x1, 0x4,  0x3, 0x5, 0x6, 0x7,  0xA, 0x9, 0x8, 0xB,  0xD, 0xC, 0xE, 0xF },
    { 0x0, 0x4, 0x2, 0x3,  0x1, 0xA, 0x9, 0x8,  0x7, 0x6, 0x5, 0xD,  0xE, 0xB, 0xC, 0xF },
};

PROGMEM static const PIECE_T pieceAryDefault[PIECES] = {
    { 0, 0, 0, 0 }, { 4, 0, 0, 0 }, { 8, 0, 0, 0 }, { 12, 0, 0, 0 }, { 16, 0, 0, 0 },
    { 0, 8, 0, 0 }, { 4, 8, 0, 0 }, { 8, 8, 0, 0 }, { 12, 8, 0, 0 }, { 16, 8, 0, 0 },
};

PROGMEM static const uint8_t imgPieceBody[16][7] = { // 7x7 x16
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },{ 0x1F, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00 },
    { 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x1F },{ 0x7C, 0x7C, 0x78, 0x70, 0x60, 0x00, 0x00 },
    { 0x00, 0x00, 0x60, 0x70, 0x78, 0x7C, 0x7C },{ 0x00, 0x7C, 0x7E, 0x7E, 0x7E, 0x7C, 0x00 },
    { 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00 },{ 0x00, 0x1F, 0x3F, 0x3F, 0x3F, 0x1F, 0x00 },
    { 0x00, 0x1C, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E },{ 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E },
    { 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x1C, 0x00 },{ 0x1F, 0x1F, 0x0F, 0x07, 0x0F, 0x1F, 0x1F },
    { 0x7F, 0x7F, 0x7F, 0x77, 0x63, 0x00, 0x00 },{ 0x00, 0x00, 0x63, 0x77, 0x7F, 0x7F, 0x7F },
    { 0x7C, 0x7C, 0x78, 0x70, 0x78, 0x7C, 0x7C },{ 0x7F, 0x7F, 0x7F, 0x77, 0x7F, 0x7F, 0x7F },
};

PROGMEM static const uint8_t imgPieceFrame[16][7] = { // 7x7 x16
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },{ 0x20, 0x20, 0x10, 0x08, 0x04, 0x03, 0x00 },
    { 0x00, 0x03, 0x04, 0x08, 0x10, 0x20, 0x20 },{ 0x02, 0x02, 0x04, 0x08, 0x10, 0x60, 0x00 },
    { 0x00, 0x60, 0x10, 0x08, 0x04, 0x02, 0x02 },{ 0x7C, 0x02, 0x01, 0x01, 0x01, 0x02, 0x7C },
    { 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F },{ 0x1F, 0x20, 0x40, 0x40, 0x40, 0x20, 0x1F },
    { 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41 },{ 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41 },
    { 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C },{ 0x20, 0x20, 0x10, 0x08, 0x10, 0x20, 0x20 },
    { 0x00, 0x00, 0x00, 0x08, 0x14, 0x63, 0x00 },{ 0x00, 0x63, 0x14, 0x08, 0x00, 0x00, 0x00 },
    { 0x02, 0x02, 0x04, 0x08, 0x04, 0x02, 0x02 },{ 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00 },
};

static uint8_t  state;
static bool     toDraw;
static int8_t   cursorX, cursorY;
static int8_t   focusPieceIdx;
static uint32_t gameFrames;
static PIECE_T  pieceAry[PIECES];
static int8_t   pieceOrder[PIECES];
static int8_t   board[BOARD_H][BOARD_W];

/*---------------------------------------------------------------------------*/
/*                              Main Functions                               */
/*---------------------------------------------------------------------------*/

void initGame(void)
{
    memcpy_P(pieceAry, pieceAryDefault, sizeof(PIECE_T) * PIECES);
    for (int i = 0; i < PIECES; i++) {
        pieceOrder[i] = i;
    }
    putPieces();
    focusPieceIdx = 0;
    gameFrames = 0;
}

bool updateGame(void)
{
    PIECE_T *p = &pieceAry[focusPieceIdx];

    int vx = 0, vy = 0;
    if (arduboy.buttonDown(LEFT_BUTTON))    vx--;
    if (arduboy.buttonDown(RIGHT_BUTTON))   vx++;
    if (arduboy.buttonDown(UP_BUTTON))      vy--;
    if (arduboy.buttonDown(DOWN_BUTTON))    vy++;

    if (arduboy.buttonPressed(A_BUTTON)) {
        if (vx) {
            p->rot = (p->rot + vx) & 3 | p->rot & 4;
        }
        if (vy) {
            p->rot = (4 - p->rot) & 3 | ~p->rot & 4;
        }
    } else {
        p->x += vx;
        p->y += vy;
    }

    if (arduboy.buttonDown(B_BUTTON)) {
        focusPieceIdx = (focusPieceIdx + 1) % PIECES;
    }

    gameFrames++;
    toDraw = true;
    return !putPieces();
}

void drawGame(void)
{
    if (toDraw) {
        arduboy.clear();
        drawBoard();
        drawPieces();
        toDraw = false;
    }
}

/*---------------------------------------------------------------------------*/
/*                             Control Functions                             */
/*---------------------------------------------------------------------------*/

static bool putPieces(void)
{
    for (int y = 0; y < BOARD_H; y++) {
        for (int x = 0; x < BOARD_W; x++) {
            board[y][x] = (x < 4 || x > 12 || (x & y & 1)) ? COLUMN : EMPTY;
        }
    }

    bool ret = false;
    for (int i = 0; i < PIECES; i++) {
        ret = putPiece(pieceOrder[i]) || ret;
    }
    return ret;
}

static bool putPiece(int8_t idx)
{
    return execPiece(idx, &putPiecePart);
}

static bool execPiece(int8_t idx, bool (*f)(int8_t, int8_t, int8_t, int8_t))
{
    bool    ret = false;
    PIECE_T *p = &pieceAry[idx];

    int8_t xStart, xEnd, yStart, yEnd;
    if (idx < 3) {
        xStart = -3; xEnd = 4;
        yStart = -1; yEnd = 0;
    } else {
        xStart = -1; xEnd = 2;
        yStart = -1; yEnd = 2;
    }
    int8_t vx = abs(p->rot % 4 - 2) - 1;
    int8_t vy = abs(p->rot % 4 - 1) - 1;
    int8_t vh = (p->rot < 4) ? 1 : -1;
    int8_t const *pPtn = piecePtn[idx];
    int8_t const *pRot = pieceRot[p->rot];
    for (int8_t dy = yStart; dy <= yEnd; dy++) {
        for (int8_t dx = xStart; dx <= xEnd; dx++) {
            int8_t x = p->x + dx * vx + dy * vy * vh;
            int8_t y = p->y - dx * vy + dy * vx * vh;
            ret = f(idx, x, y, pgm_read_byte(pRot + pgm_read_byte(pPtn++))) || ret;
        }
    }
    return ret;

}

static bool putPiecePart(int8_t idx, int8_t x, int8_t y, int8_t c)
{
    bool ret = false;
    if (c >= 5) {
        if (x >= 0 && y >= 0 && x < BOARD_W && y < BOARD_H) {
            int8_t b = board[y][x];
            if (b != EMPTY && b != idx) {
                ret = true;
            }
            board[y][x] = idx;
        } else {
            ret = true;
        }
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                              Draw Functions                               */
/*---------------------------------------------------------------------------*/

static void drawBoard(void)
{
    arduboy.drawFastVLine2(31, 0, 64, WHITE);
    arduboy.drawFastVLine2(95, 0, 64, WHITE);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            arduboy.drawRect2(x * 14 + 39, y * 14 + 7, 7, 7, WHITE);
        }
    }
}

static void drawPieces(void)
{
    for (int i = 0; i < PIECES; i++) {
        execPiece(pieceOrder[i], &drawPiecePart);
    }
}

static void drawPiece(int8_t idx)
{
    execPiece(idx, &drawPiecePart);
}

static bool drawPiecePart(int8_t idx, int8_t x, int8_t y, int8_t c)
{
    uint8_t bodyCol = WHITE, frameCol = BLACK;
    if (idx == focusPieceIdx) {
        bodyCol = gameFrames & 1;
        frameCol = (gameFrames & 2) >> 1;
    }
    arduboy.drawBitmap(x * 7 + 4, y * 7, imgPieceBody[c], 7, 7, bodyCol);
    arduboy.drawBitmap(x * 7 + 4, y * 7, imgPieceFrame[c], 7, 7, frameCol);
    return false;
}
