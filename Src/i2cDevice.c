#include "i2cDevice.h"

const uint8_t bootTimeCheckPos[6] = {
    ((__TIME__[0]) - '0' + 128 + 0),  ((__TIME__[1]) - '0' + 128 + 10), ((__TIME__[3]) - '0' + 128 + 20),
    ((__TIME__[4]) - '0' + 128 + 30), ((__TIME__[7]) - '0' + 128 + 40), ((__TIME__[8]) - '0' + 128 + 50),
};

uint8_t eeRead(uint8_t addr)
{
    uint8_t dat;

    i2cStart();
    i2cSend(0xa0);
    i2cWaitAck();
    i2cSend(addr);
    i2cWaitAck();

    i2cStart();
    i2cSend(0xa1);
    i2cWaitAck();
    dat = i2cRead();
    i2cNak();
    i2cStop();

    return dat;
}

void eeWrite(uint8_t addr, uint8_t dat)
{
    i2cStart();
    i2cSend(0xa0);
    i2cWaitAck();
    i2cSend(addr);
    i2cWaitAck();
    i2cSend(dat);
    i2cWaitAck();
    i2cStop();
    HAL_Delay(6);
}

uint8_t mcpRead(void)
{
    uint8_t val;
    i2cStart();
    i2cSend(0x5f);
    i2cWaitAck();
    val = i2cRead();
    i2cNak();
    i2cStop();
    return val;
}

void mcpWrite(uint8_t dat)
{
    i2cStart();
    i2cSend(0x5e);
    i2cWaitAck();
    i2cSend(dat);
    i2cWaitAck();
    i2cStop();
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
