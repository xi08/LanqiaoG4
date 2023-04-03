#include "ee_mcp.h"

/**
 * @brief read MCP4017 value
 *
 * @return uint8_t value
 */
uint8_t mcpRead(void)
{
    uint8_t val;
    I2CStart();
    I2CSendByte(0x5f);
    I2CWaitAck();
    val = I2CReceiveByte();
    I2CSendNotAck();
    I2CStop();
    return val;
}

/**
 * @brief write MCP4017 value
 *
 * @param val value
 */
void mcpWrite(uint8_t val)
{
    I2CStart();
    I2CSendByte(0x5e);
    I2CWaitAck();
    I2CSendByte(val);
    I2CWaitAck();
    I2CStop();
}

/**
 * @brief read byte from e2prom
 *
 * @param addr address in e2prom
 * @return uint8_t data
 */
uint8_t eeRead(uint8_t addr)
{
    uint8_t val;
    I2CStart();
    I2CSendByte(0xa0);
    I2CWaitAck();
    I2CSendByte(addr);
    I2CWaitAck();
    I2CStart();
    I2CSendByte(0xa1);
    I2CWaitAck();
    val = I2CReceiveByte();
    I2CWaitAck();
    I2CStop();
    return val;
}

/**
 * @brief write byte to e2prom
 *
 * @param addr address in e2prom
 * @param dat data
 */
void eeWrite(uint8_t addr, uint8_t dat)
{
    I2CStart();
    I2CSendByte(0xa0);
    I2CWaitAck();
    I2CSendByte(addr);
    I2CWaitAck();
    I2CSendByte(dat);
    I2CWaitAck();
    I2CStop();
}

/**
 * @brief read page from e2prom
 *
 * @param addr address in e2prom
 * @param buf buffer address
 * @param n number
 */
void eeReadP(uint8_t addr, uint8_t *buf, uint8_t n)
{
    I2CStart();
    I2CSendByte(0xa0);
    I2CWaitAck();
    I2CSendByte(addr);
    I2CWaitAck();
    I2CStart();
    I2CSendByte(0xa1);
    I2CWaitAck();
    while (n--)
    {
        *buf++ = I2CReceiveByte();
        if (n)
            I2CSendAck();
        else
            I2CSendNotAck();
    }
    I2CStop();
}

/**
 * @brief write page to e2prom
 *
 * @param addr address in e2prom
 * @param dat data address
 * @param n num
 */
void eeWriteP(uint8_t addr, uint8_t *dat, uint8_t n)
{
    I2CStart();
    I2CSendByte(0xa0);
    I2CWaitAck();
    I2CSendByte(addr);
    I2CWaitAck();
    while (n--)
    {
        I2CSendByte(*dat++);
        I2CWaitAck();
    }
    I2CStop();
    HAL_Delay(6);
}
