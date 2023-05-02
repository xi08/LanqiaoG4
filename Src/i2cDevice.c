#include "i2cDevice.h"

const uint8_t bootTimeCheckPos[6] = {
    ((__TIME__[0]) - '0' + 128 + 0),  ((__TIME__[1]) - '0' + 128 + 10), ((__TIME__[3]) - '0' + 128 + 20),
    ((__TIME__[4]) - '0' + 128 + 30), ((__TIME__[7]) - '0' + 128 + 40), ((__TIME__[8]) - '0' + 128 + 50),
};

uint8_t eeRead(uint8_t addr)
{
    uint8_t dat;

    I2C_Start();
    I2C_SendByte(0xa0);
    I2C_WaitACK();
    I2C_SendByte(addr);
    I2C_WaitACK();

    I2C_Start();
    I2C_SendByte(0xa1);
    I2C_WaitACK();
    dat = I2C_ReadByte();
    I2C_SendNAK();
    I2C_Stop();

    return dat;
}

void eeWrite(uint8_t addr, uint8_t dat)
{
    I2C_Start();
    I2C_SendByte(0xa0);
    I2C_WaitACK();
    I2C_SendByte(addr);
    I2C_WaitACK();
    I2C_SendByte(dat);
    I2C_WaitACK();
    I2C_Stop();
    HAL_Delay(6);
}

uint8_t mcpRead(void)
{
    uint8_t val;
    I2C_Start();
    I2C_SendByte(0x5f);
    I2C_WaitACK();
    val = I2C_ReadByte();
    I2C_SendNAK();
    I2C_Stop();
    return val;
}

void mcpWrite(uint8_t dat)
{
    I2C_Start();
    I2C_SendByte(0x5e);
    I2C_WaitACK();
    I2C_SendByte(dat);
    I2C_WaitACK();
    I2C_Stop();
}

uint8_t bootCheck(void)
{
    uint8_t bT0, bTmp, i;

    bT0 = eeRead(bootTimeCheckPos[0]); // Read 1st check pos.

    for (i = 1; i < 6; i++) // Read the rest pos.
    {
        bTmp = eeRead(bootTimeCheckPos[i]);
        if (bT0 != bTmp) // error
        {
            bT0 = 0; // first boot
            break;
        }
    }

    bT0++; // bootTime inc.

    for (i = 0; i < 6; i++) // Write to check pos.
        eeWrite(bootTimeCheckPos[i], bT0);

    return bT0;
}
