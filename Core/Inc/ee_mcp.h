#ifndef __EE_MCP_H
#define __EE_MCP_H


#include "i2c.h"
#define i2c_start() (I2CStart())
#define i2c_stop() (I2CStop())
#define i2c_ack() (I2CSendAck())
#define i2c_nak() (I2CSendNotAck())
#define i2c_wack() (I2CWaitAck())
#define i2c_send(x) (I2CSendByte((x)))
#define i2c_receive() (I2CReceiveByte())
#endif

uint8_t eeRead(uint8_t addr);
void eeReadP(uint8_t addr, uint8_t *buf, uint8_t n);
void eeWrite(uint8_t addr, uint8_t dat);
void eeWriteP(uint8_t addr, uint8_t *dat, uint8_t n);
uint8_t mcpRead(void);
void mcpWrite(uint8_t val);
