#ifndef __IIC2_H
#define __IIC2_H

#include "main.h"

void i2cReset(void);
void i2cSTART(void);
void i2cSTOP(void);
void i2cACK(void);
void i2cNAK(void);
uint8_t i2cWaitACK(void);
void i2cSend(uint8_t data);
uint8_t i2cReceive(void);

#endif
