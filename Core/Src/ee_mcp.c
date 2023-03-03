#include "ee_mcp.h"

/**
 * @brief read MCP4017 value
 *
 * @return uint8_t value
 */
uint8_t mcpRead(void)
{
    uint8_t val;
    i2c_start();
    i2c_send(0x5f);
    i2c_wack();
    val = i2c_receive();
    i2c_nak();
    i2c_stop();
    return val;
}

/**
 * @brief write MCP4017 value
 *
 * @param val value
 */
void mcpWrite(uint8_t val)
{
    i2c_start();
    i2c_send(0x5e);
    i2c_wack();
    i2c_send(val);
    i2c_wack();
    i2c_stop();
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
    i2c_start();
    i2c_send(0xa0);
    i2c_wack();
    i2c_send(addr);
    i2c_wack();
    i2c_start();
    i2c_send(0xa1);
    i2c_wack();
    val = i2c_receive();
    i2c_wack();
    i2c_stop();
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
    i2c_start();
    i2c_send(0xa0);
    i2c_wack();
    i2c_send(addr);
    i2c_wack();
    i2c_send(dat);
    i2c_wack();
    i2c_stop();
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
    i2c_start();
    i2c_send(0xa0);
    i2c_wack();
    i2c_send(addr);
    i2c_wack();
    i2c_start();
    i2c_send(0xa1);
    i2c_wack();
    while (n--)
    {
        *buf++ = i2c_receive();
        if (n)
            i2c_ack();
        else
            i2c_nak();
    }
    i2c_stop();
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
    i2c_start();
    i2c_send(0xa0);
    i2c_wack();
    i2c_send(addr);
    i2c_wack();
    while (n--)
    {
        i2c_send(*dat++);
        i2c_wack();
    }
    i2c_stop();
    msDelay(6);
}
