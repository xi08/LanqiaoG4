#include "seg7.h"

const uint8_t seg7Mask[20] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                              0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00, 0xff, 0x40, 0x80};

void segInit(void)
{
    uint8_t i = 24;
    LL_GPIO_ResetOutputPin(RCK_GPIO_Port, RCK_Pin);
    while (i--)
    {
        LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        LL_GPIO_ResetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
    }
    LL_GPIO_SetOutputPin(RCK_GPIO_Port, RCK_Pin);
}

void segSend(uint8_t pos1, uint8_t pos2, uint8_t pos3)
{
    uint8_t i;

    LL_GPIO_ResetOutputPin(RCK_GPIO_Port, RCK_Pin), delay_us(1);

    // pos3
    for (i = 0; i < 8; i++)
    {
        LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        if (pos3 & 0x80)
            LL_GPIO_SetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        else
            LL_GPIO_ResetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        pos3 <<= 1;
    }
    // pos2
    for (i = 0; i < 8; i++)
    {
        LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        if (pos2 & 0x80)
            LL_GPIO_SetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        else
            LL_GPIO_ResetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        pos2 <<= 1;
    }
    // pos1
    for (i = 0; i < 8; i++)
    {
        LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        if (pos1 & 0x80)
            LL_GPIO_SetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        else
            LL_GPIO_ResetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        pos1 <<= 1;
    }
    LL_GPIO_SetOutputPin(RCK_GPIO_Port, RCK_Pin), delay_us(1);
}