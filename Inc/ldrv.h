#ifndef __LDRV_H
#define __LDRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef enum
{
    lcdDispFR_30 = 0x0000,
    lcdDispFR_50 = 0x0008,
    lcdDispFR_60 = 0x000a,
    lcdDispFR_90 = 0x000d,
} lcdDispFR_t;

#define Line0 0
#define Line1 24
#define Line2 48
#define Line3 72
#define Line4 96
#define Line5 120
#define Line6 144
#define Line7 168
#define Line8 192
#define Line9 216

void lcdInit(lcdDispFR_t lfr);

void lcdSetFrontColor(uint16_t lc);
void lcdSetBackColor(uint16_t lc);

void lcdSetCursor(uint16_t Xpos, uint16_t Ypos);
void lcdSetUpdateWindow(uint16_t Xst, uint16_t Yst, uint16_t Xed, uint16_t Yed);

void lcdClearScreen(void);
void lcdClearLine(uint16_t Line);

void lcdDisplayString(uint16_t Line, char *str);
void lcdDisplayChar(uint16_t Xpos, uint16_t Ypos, char ch);

#ifdef __cplusplus
}
#endif


#endif // !LDRV_H