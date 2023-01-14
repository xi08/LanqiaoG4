#ifndef __LCD_H
#define __LCD_H

#include "main.h"

typedef volatile uint16_t lcdColor_t;

#define Red 0xF800
#define Green 0x07E0
#define Blue 0x001F
#define White 0xFFFF
#define Cyan 0x7FFF
#define Magenta 0xF81F
#define Yellow 0xFFE0
#define Black 0x0000
#define Grey 0xF7DE

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

#define Horizontal 0x00
#define Vertical 0x01

void LCD_init(void);
void LCD_setFrontColor(lcdColor_t color);
void LCD_setBackColor(lcdColor_t color);
void LCD_clear(lcdColor_t color);
void LCD_setCursor(uint8_t Xpos, uint16_t Ypos);




#endif
