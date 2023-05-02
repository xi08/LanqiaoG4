#include "ds18b20.h"
#include "swDelay.h"

uint8_t DS18B20_buffer[9];

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

void DS18B20_WriteByte(uint8_t dat)
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

uint8_t DS18B20_ReadByte(void)
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

uint8_t DS18B20_Read(float *temp)
{
    uint8_t i, errTime;
    int16_t t;

    // Start convert
    errTime = 6;
    while (--errTime)
    {
        if (!DS18B20_Reset())
            continue;

        DS18B20_WriteByte(0xcc);
        DS18B20_WriteByte(0x44);
        break;
    }
    if (!errTime)
        return 1;

    // Delay time
    HAL_Delay(775);

    // Read data
    errTime = 6;
    while (--errTime)
    {
        if (!DS18B20_Reset())
            continue;

        DS18B20_WriteByte(0xcc);
        DS18B20_WriteByte(0xbe);

        for (i = 0; i < 9; i++)
            DS18B20_buffer[i] = DS18B20_ReadByte();

        if (DS18B20_CRC(DS18B20_buffer, 8) == DS18B20_buffer[8])
            break;
    }
    if (!errTime)
        return 1;

    // Combine data
    t = DS18B20_buffer[1];
    t <<= 8;
    t |= DS18B20_buffer[0];

    // Output data
    (*temp) = t * 1.0 / 16;

    return 0;
}
