#ifndef __I2CDEVICE_H

#include "main.h"
#include "i2c.h"

uint8_t eeRead(uint8_t addr);
void eeWrite(uint8_t addr, uint8_t dat);
uint8_t mcpRead(void);
void mcpWrite(uint8_t dat);
uint8_t bootCheck(void);

#endif // !__I2CDEVICE_H
