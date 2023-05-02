#include "i2c.h"
#include "swDelay.h"

void I2C_Init(void)
{
    uint8_t i = 9;
    while (i--)
    {
        LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
        LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    }
}

void I2C_Start(void)
{
    LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2);   // SDA=1
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

void I2C_Stop(void)
{
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
    LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2);   // SDA=1
}

uint8_t I2C_WaitACK(void)
{
    uint8_t errTime = 5;
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=1
    while (LL_GPIO_IsInputPinSet(SDA_GPIO_Port, SDA_Pin))
    {
        errTime--;
        if (!errTime)
        {
            I2C_Stop();
            return 1;
        }
    }
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
    return 0;
}

void I2C_SendACK(void)
{
    LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

void I2C_SendNAK(void)
{
    LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2);   // SDA=1
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

void I2C_SendByte(uint8_t dat)
{
    uint8_t i = 8;
    while (i--)
    {
        LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
        if (dat & 0x80)
            LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=1
        else
            LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
        LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);       // SCL=1
        dat <<= 1;
    }
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

uint8_t I2C_ReadByte(void)
{
    uint8_t i = 8, dat = 0;

    while (i--)
    {
        dat <<= 1;
        LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
        LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
        dat |= LL_GPIO_IsInputPinSet(SDA_GPIO_Port, SDA_Pin);
    }
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
    return dat;
}
