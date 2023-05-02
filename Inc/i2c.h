#ifndef __I2C_H
#define __I2C_H

#include "main.h"

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_WaitACK(void);
void I2C_SendACK(void);
void I2C_SendNAK(void);
void I2C_SendByte(uint8_t cSendByte);
uint8_t I2C_ReadByte(void);

#endif // !__I2C_H
