#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

void i2cInit(void);
void i2cStart(void);
void i2cStop(void);
uint8_t i2cWaitAck(void);
void i2cAck(void);
void i2cNak(void);
void i2cSend(uint8_t cSendByte);
uint8_t i2cRead(void);

#ifdef __cplusplus
}
#endif


#endif // !__I2C_H
