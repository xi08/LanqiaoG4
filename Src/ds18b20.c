#include "ds18b20.h"
#include "swDelay.h"

uint8_t DS18B20_buffer[9], DS18B20_Resolution;

uint8_t DS18B20_Reset(void)
{
    uint8_t dat;

    LL_GPIO_SetOutputPin(OWD_GPIO_Port, OWD_Pin);
    delay_us(20);
    LL_GPIO_ResetOutputPin(OWD_GPIO_Port, OWD_Pin);
    delay_us(750);
    LL_GPIO_SetOutputPin(OWD_GPIO_Port, OWD_Pin);
    delay_us(90);
    dat = LL_GPIO_IsInputPinSet(OWD_GPIO_Port, OWD_Pin);
    delay_us(400);

    return dat;
}

void DS18B20_Send(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        LL_GPIO_ResetOutputPin(OWD_GPIO_Port, OWD_Pin);
        delay_us(5);
        if (!(dat & 1))
            delay_us(55);
        LL_GPIO_SetOutputPin(OWD_GPIO_Port, OWD_Pin);
        delay_us(5);
        if (dat & 1)
            delay_us(55);
        dat >>= 1;
    }
}

uint8_t DS18B20_Read(void)
{
    uint8_t i, result = 0;
    for (i = 0; i < 8; i++)
    {
        result >>= 1;
        LL_GPIO_ResetOutputPin(OWD_GPIO_Port, OWD_Pin);
        delay_us(5);
        LL_GPIO_SetOutputPin(OWD_GPIO_Port, OWD_Pin);
        delay_us(7);
        if (LL_GPIO_IsInputPinSet(OWD_GPIO_Port, OWD_Pin))
            result |= 0x80;
        delay_us(53);
    }
    return result;
}

uint8_t DS18B20_CRC(uint8_t *dataBlock, size_t blockSize)
{
    uint8_t i, crc = 0;
    while (blockSize--)
    {
        crc ^= *dataBlock++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8c;
            else
                crc >>= 1;
        }
    }
    return crc;
}

uint8_t DS18B20_Start(uint8_t Res)
{
    uint8_t errTime = 6;

    while (--errTime)
    {
        if (!DS18B20_Reset())
            continue;

        DS18B20_Send(0xcc);
        DS18B20_Send(0x4e);
        DS18B20_Send(100);
        DS18B20_Send(0);
        DS18B20_Send(0x1f | ((Res - 9) << 5));
        break;
    }
    if (!errTime)
        return 1;

    DS18B20_Resolution = 12 - Res;
    return 0;
}

uint8_t DS18B20_Conv(float *temp)
{
    uint8_t i, errTime = 6;
    int16_t t;

    // Read data
    while (--errTime)
    {
        if (!DS18B20_Reset())
            continue;

        DS18B20_Send(0xcc);
        DS18B20_Send(0xbe);

        for (i = 0; i < 9; i++)
            DS18B20_buffer[i] = DS18B20_Read();

        if (DS18B20_CRC(DS18B20_buffer, 8) == DS18B20_buffer[8])
            break;
    }
    if (!errTime)
        return 1;

    // Combine data
    t = DS18B20_buffer[1];
    t <<= 8;
    t |= DS18B20_buffer[0];

    // Convert data
    t >>= DS18B20_Resolution;
    (*temp) = t * 1.0 / (1 << (4 - DS18B20_Resolution));

    return 0;
}
