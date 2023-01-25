#ifndef __LCD_H
#define __LCD_H

#include "main.h"

typedef volatile uint16_t lcdColor_t;
typedef const uint16_t lcdFont_t;

#define Red 0xF800
#define Green 0x07E0
#define Blue 0x001F
#define White 0xFFFF
#define Cyan 0x7FFF
#define Magenta 0xF81F
#define Yellow 0xFFE0
#define Black 0x0000
#define Grey 0xF7DE

#define Row0 0
#define Row1 24
#define Row2 48
#define Row3 72
#define Row4 96
#define Row5 120
#define Row6 144
#define Row7 168
#define Row8 192
#define Row9 216

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
