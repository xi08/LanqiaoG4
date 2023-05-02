#include "dht11.h"
#include "swDelay.h"

uint8_t DHT11_ReadData(float *humi, float *temp)
{
    uint8_t dat[5]={0}, i, j;

    LL_GPIO_ResetOutputPin(DHT_GPIO_Port, DHT_Pin); // DQ=0
    HAL_Delay(20);
    LL_GPIO_SetOutputPin(DHT_GPIO_Port, DHT_Pin); // DQ=1
    delay_us(30);
    
    if (LL_GPIO_IsInputPinSet(DHT_GPIO_Port, DHT_Pin))
        return 1;
    while (!LL_GPIO_IsInputPinSet(DHT_GPIO_Port, DHT_Pin))
        ;
    while (LL_GPIO_IsInputPinSet(DHT_GPIO_Port, DHT_Pin))
        ;

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 8; j++)
        {
            dat[i] <<= 1;
            while (!LL_GPIO_IsInputPinSet(DHT_GPIO_Port, DHT_Pin))
                ;
            delay_us(30);
            dat[i] |= LL_GPIO_IsInputPinSet(DHT_GPIO_Port, DHT_Pin);
            while (LL_GPIO_IsInputPinSet(DHT_GPIO_Port, DHT_Pin))
                ;
        }
    }

    if ((dat[0] + dat[1] + dat[2] + dat[3]) == dat[4])
    {
        (*humi) = dat[0] + 0.1 * dat[1];
        (*temp) = dat[2] + 0.1 * (dat[3] & 0x7f);
        if (dat[3] & 0x80)
            (*temp) *= -1;
    }
    else
        return 1;

    return 0;
}