#include "seg7.h"
#include "swDelay.h"

const uint8_t seg7Mask[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f,
                              0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00};

void seg7Init(void)
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

void seg7Write(uint8_t bit1, uint8_t bit2, uint8_t bit3)
{
    uint8_t i, codeTmp;

    LL_GPIO_ResetOutputPin(RCK_GPIO_Port, RCK_Pin), delay_us(1);

    // Pos3
    codeTmp = seg7Mask[bit3];
    for (i = 0; i < 8; i++)
    {
        LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        if (codeTmp & 0x80)
            LL_GPIO_SetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        else
            LL_GPIO_ResetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        codeTmp <<= 1;
    }

    // Pos2
    codeTmp = seg7Mask[bit2];
    for (i = 0; i < 8; i++)
    {
        LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        if (codeTmp & 0x80)
            LL_GPIO_SetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        else
            LL_GPIO_ResetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        codeTmp <<= 1;
    }

    // Pos1
    codeTmp = seg7Mask[bit1];
    for (i = 0; i < 8; i++)
    {
        LL_GPIO_ResetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        if (codeTmp & 0x80)
            LL_GPIO_SetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        else
            LL_GPIO_ResetOutputPin(SER_GPIO_Port, SER_Pin), delay_us(1);
        LL_GPIO_SetOutputPin(SCK_GPIO_Port, SCK_Pin), delay_us(1);
        codeTmp <<= 1;
    }
    LL_GPIO_SetOutputPin(RCK_GPIO_Port, RCK_Pin), delay_us(1);
}