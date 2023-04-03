#ifndef __EE_MCP_H
#define __EE_MCP_H

#include "i2c.h"

uint8_t eeRead(uint8_t addr);
void eeReadP(uint8_t addr, uint8_t *buf, uint8_t n);
void eeWrite(uint8_t addr, uint8_t dat);
void eeWriteP(uint8_t addr, uint8_t *dat, uint8_t n);
uint8_t mcpRead(void);
void mcpWrite(uint8_t val);

#endif
