#include "iic2.h"

#define i2cDelay() usDelay(10)

inline uint32_t i2cSDAInput(void)
{
    return LL_GPIO_IsInputPinSet(SDA_GPIO_Port, SDA_Pin);
}

inline void i2cSCLOutput(uint8_t x)
{
    if (x)
        LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin);
    else
        LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin);
}

inline void i2cSDAOutput(uint8_t x)
{
    if (x)
        LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin);
    else
        LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin);
}

/**
 * @brief Bus Reset
 *
 */
void i2cReset(void)
{
    uint8_t i;

    /* 连续发送9个时钟 */
    for (i = 0; i < 9; i++)
    {
        i2cSCLOutput(1);
        i2cDelay();
        i2cSCLOutput(0);
        i2cDelay();
        if (i2cSDAInput())
            return;
    }
}

/**
 * @brief Send I2C Start Signal
 *
 */
void i2cSTART(void)
{
    i2cSDAOutput(1);
    i2cSCLOutput(1);
    i2cDelay();
    i2cSDAOutput(0);
    i2cDelay();
    i2cSCLOutput(0);
}

/**
 * @brief Send I2C Stop Signal
 *
 */
void i2cSTOP(void)
{
    i2cSDAOutput(0);
    i2cSCLOutput(1);
    i2cDelay();
    i2cSDAOutput(1);
    i2cDelay();
    i2cSCLOutput(0);
}

/**
 * @brief Send I2C ACK Signal
 *
 */
void i2cACK(void)
{
    i2cSDAOutput(0);
    i2cDelay();
    i2cSCLOutput(1);
    i2cDelay();
    i2cSCLOutput(0);
    i2cDelay();
    i2cSDAOutput(1);
}

/**
 * @brief Send I2C NACK Signal
 *
 */
void i2cNACK(void)
{
    i2cSDAOutput(1);
    i2cDelay();
    i2cSCLOutput(1);
    i2cDelay();
    i2cSCLOutput(0);
    i2cDelay();
}

/**
 * @brief Wait for I2C ACK Signal
 *
 * @return uint8_t ACK Signal
 */
uint8_t i2cWaitACK(void)
{
    uint8_t errTime = 5;
    i2cInputMode();
    i2cDelay();
    i2cSCLOutput(1);
    i2cDelay();
    while (i2cSDAInput())
    {
        errTime--;
        i2cDelay();
        if (!errTime)
        {
            i2cOutputMode();
            i2cSTOP();
            return 1;
        }
    }
    i2cSCLOutput(0);
    i2cDelay();
    i2cOutputMode();
    return 0;
}

/**
 * @brief Send a byte in bus
 *
 * @param dat Transferred Data
 */
void i2cSend(uint8_t dat)
{
    uint8_t i;
    i2cOutputMode();
    for (i = 0; i < 8; i++)
    {
        i2cSCLOutput(0);
        i2cDelay();
        i2cSDAOutput(dat & 0x80);
        dat <<= 1;
        i2cDelay();
        i2cSCLOutput(1);
        i2cDelay();
    }
    i2cSCLOutput(0);
    i2cDelay();
}

/**
 * @brief Receive a byte in bus
 *
 * @return uint8_t Received Data
 */
uint8_t i2cReceive(void)
{
    uint8_t i;
    uint8_t data = 0;
    i2cInputMode();
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        i2cSCLOutput(1);
        i2cDelay();
        data |= i2cSDAInput();
        i2cSCLOutput(0);
        i2cDelay();
    }
    i2cOutputMode();
    return data;
}
