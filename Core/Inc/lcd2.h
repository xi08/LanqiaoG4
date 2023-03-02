#ifndef __LCD2_H
#define __LCD2_H

#include "main.h"

typedef volatile uint16_t lcdColor_t;
typedef const uint16_t lcdFont_t;

#define lcdColor_Red 0xF800
#define lcdColor_Green 0x07E0
#define lcdColor_Blue 0x001F
#define lcdColor_White 0xFFFF
#define lcdColor_Cyan 0x7FFF
#define lcdColor_Magenta 0xF81F
#define lcdColor_Yellow 0xFFE0
#define lcdColor_Black 0x0000
#define lcdColor_Grey 0xF7DE

#define lcdDisp_Row0 0
#define lcdDisp_Row1 24
#define lcdDisp_Row2 48
#define lcdDisp_Row3 72
#define lcdDisp_Row4 96
#define lcdDisp_Row5 120
#define lcdDisp_Row6 144
#define lcdDisp_Row7 168
#define lcdDisp_Row8 192
#define lcdDisp_Row9 216

void LCD_init(void);
void LCD_setFrontColor(lcdColor_t _Color);
void LCD_setBackColor(lcdColor_t _Color);
void LCD_clear(lcdColor_t _Color);
void LCD_clearRow(uint8_t _Row);
void LCD_dispChar(uint8_t _Row, uint16_t _Col, char _Ch);
void LCD_dispString(uint8_t _Row, char *_Str);
void LCD_dispLine(uint8_t Xst, uint16_t Yst, uint8_t Xed, uint16_t Yed);
void LCD_dispRect(uint8_t Xst, uint16_t Yst, uint8_t Xed, uint16_t Yed);
void LCD_dispCircle(uint8_t Xpos, uint16_t Ypos, uint16_t _Radius);
void LCD_dispMonoPic(const uint32_t *_Pic);
void LCD_dispColorPic(void);

#endif
