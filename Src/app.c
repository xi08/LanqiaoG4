#include "adc.h"
#include "dht11.h"
#include "dma.h"
#include "ds18b20.h"
#include "gpio.h"
#include "i2c.h"
#include "i2cDevice.h"
#include "lcd.h"
#include "main.h"
#include "seg7.h"
#include "tim.h"
#include "usart.h"

uint8_t bootTime;

uint8_t akyNum;
uint16_t r39Freq, r40Freq;

uint8_t MCP_CFGVal;

uint32_t sTimerTS_lcdRefresh;
uint8_t dispBuffer[21];

uint32_t sTimerTS_dht11Refresh;
float DHT11_Temp, DHT11_Humi;

uint32_t sTimerTS_ds18b20Update;
uint8_t DS18B20_Mode;
float DS18B20_Temp;

float r37Val, r38Val, mcpVal, akyVal, traVal;

void setup(void)
{
    // Boot time check
    i2cInit();
    bootTime = bootCheck();

    // LCD init.
    LCD_Init(), LCD_Clear(Black);
    LCD_SetTextColor(White), LCD_SetBackColor(Black);

    // LED init.
    ledUpdate(bootTime);

    // Seg. init.
    segInit();
    segSend(seg7Mask[bootTime / 100], seg7Mask[(bootTime / 10) % 10], seg7Mask[bootTime % 10]);

    // Display boot info.
    sprintf((char *)dispBuffer, "CT117E-M4  Boot=%u", bootTime);
    LCD_DisplayStringLine(Line0, dispBuffer);
    LCD_DisplayStringLine(Line1, (uint8_t *)"Compiled in");
    sprintf((char *)dispBuffer, "%s,%s", __TIME__, __DATE__);
    LCD_DisplayStringLine(Line2, dispBuffer);

    // ADC start
    adcConvStart();

    // UART start, DMA2CH1
    if (HAL_UART_Receive_DMA(&huart1, uartBuffer, sizeof(uartBuffer)) != HAL_OK)
        Error_Handler();
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    HAL_Delay(2000);

    // Clear display
    segSend(seg7Mask[16], seg7Mask[16], seg7Mask[16]);
    LCD_Clear(Black);
    ledUpdate(0);
    sTimerTS_lcdRefresh = HAL_GetTick();
}

void loop(void)
{
    uint8_t errTime;
    // Check ADC Conv. Value
    adcConvCheck(&r37Val, &r38Val, &mcpVal, &akyVal, &traVal);

    // Key Scan & AKEY Conv.
    keyScan();

    // MCP4017 Read
    MCP_CFGVal = mcpRead();
#ifdef CT117EXA
    // DHT11 Read
    if (HAL_GetTick() - sTimerTS_dht11Refresh > 2000)
    {
        sTimerTS_dht11Refresh = HAL_GetTick();
        errTime = 5;
        while (DHT11_ReadData(&DHT11_Humi, &DHT11_Temp) || !errTime)
            errTime--;

        if (!errTime)
            Error_Handler();
    }

    // DS18B20 Read
    if (HAL_GetTick() - sTimerTS_ds18b20Update > 800)
    {
        sTimerTS_ds18b20Update = HAL_GetTick();
        errTime = 5;
        if (DS18B20_Mode)
        {
            while (DS18B20_Start() || !errTime)
                errTime--;
            if (!errTime)
                Error_Handler();
            DS18B20_Mode = 0;
        }
        else
        {
            while (DS18B20_Conv(&DS18B20_Temp) || !errTime)
                errTime--;
            if (!errTime)
                Error_Handler();
            DS18B20_Mode = 1;
        }
    }
#endif

    // Key Action
    keyProg();

    // LED Display
    ledUpdate(ledBuffer);

    // LCD Display
    if (HAL_GetTick() - sTimerTS_lcdRefresh > 50)
    {
        sTimerTS_lcdRefresh = HAL_GetTick();

        printf("\n");

        LCD_ClearLine(Line0);
        sprintf((char *)dispBuffer, "sM=");
        LCD_DisplayStringLine(Line0, dispBuffer), printf("%s\n", dispBuffer);

        LCD_ClearLine(Line1);
        sprintf((char *)dispBuffer, "A1=%.2fV,F1=%uHz", r37Val, r39Freq);
        LCD_DisplayStringLine(Line1, dispBuffer), printf("%s\n", dispBuffer);

        LCD_ClearLine(Line2);
        sprintf((char *)dispBuffer, "A2=%.2fV,F2=%uHz", r38Val, r40Freq);
        LCD_DisplayStringLine(Line2, dispBuffer), printf("%s\n", dispBuffer);

        LCD_ClearLine(Line3);
        sprintf((char *)dispBuffer, "Res=%.2fV,%.2fKOhm", mcpVal, MCP_CFGVal * 0.7874);
        LCD_DisplayStringLine(Line3, dispBuffer), printf("%s\n", dispBuffer);
    }

    // MCP4017 Write
    mcpWrite(MCP_CFGVal);
}

void keyProg(void)
{
    /* B1 Short */
    if (keyState[0] == S2)
    {
        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B1S");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // act.
        ledBuffer <<= 1;
        if (ledBuffer == 0x0)
            ledBuffer = 0x01;

        // reset state
        keyState[0] = S0;
    }

    /* B1 Long */
    if (keyState[0] == S4)
    {
        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B1L");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // act.
        ledBuffer = 0x00;

        // reset state
        keyState[1] = S0;
    }

    /* B2 Short */
    if (keyState[1] == S2)
    {
        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B2S");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // act.
        ledBuffer >>= 1;
        if (ledBuffer == 0x0)
            ledBuffer = 0x80;

        // reset state
        keyState[0] = S0;
    }

    /* B2 Long */
    if (keyState[1] == S4)
    {
        // act.
        ledBuffer = 0xff;

        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B2L");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // reset state
        keyState[1] = S0;
    }

    /* B3 Short */
    if (keyState[2] == S2)
    {
        // act.
        mcpWrite(--MCP_CFGVal);

        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B3S");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // reset state
        keyState[2] = S0;
    }

    /* B3 Long */
    if (keyState[2] == S4)
    {
        // act.
        MCP_CFGVal = 0;
        mcpWrite(MCP_CFGVal);

        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B3L");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // reset state
        keyState[2] = S0;
    }

    /* B4 Short */
    if (keyState[3] == S2)
    {

        // act.
        mcpWrite(++MCP_CFGVal);

        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B4S");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // reset state
        keyState[3] = S0;
    }

    /* B4 Long */
    if (keyState[3] == S4)
    {
        // disp.
        LCD_ClearLine(Line9);
        sprintf((char *)dispBuffer, "Key=%s", "B4L");
        LCD_DisplayStringLine(Line9, dispBuffer);

        // act.
        MCP_CFGVal = 0x7f;
        mcpWrite(MCP_CFGVal);

        // reset state
        keyState[3] = S0;
    }
}
