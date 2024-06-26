#include "adc.h"
#include "dht11.h"
#include "dma.h"
#include "ds18b20.h"
#include "gpio.h"
#include "i2c.h"
#include "i2cDevice.h"
#include "ldrv.h"
#include "main.h"
#include "seg7.h"
#include "tim.h"
#include "usart.h"

#include <iostream>

typedef enum
{
    sMode_M4,
    sMode_EXA,

} sMode_t;

typedef enum
{
    kMode_MCP,
    kMode_LED,
    kMode_DS,
    kMode_SEG,

} kMode_t;

uint8_t bootTime;

kMode_t kMode = kMode_MCP;
sMode_t sMode = sMode_M4;
char sModeText[6] = "M4", kModeText[11] = "MCP";

uint8_t akyNum;
extern uint16_t r39Freq, r40Freq;

uint8_t MCP_CFGVal;

uint32_t sTimerTS_lcdRefresh;
char dispBuffer[21];

uint32_t sTimerTS_dht11Refresh;
float DHT11_Temp, DHT11_Humi;

uint32_t sTimerTS_ds18b20Update;
uint8_t DS18B20_Mode, DS18B20_Res = 12;
float DS18B20_Temp;

float r37Val, r38Val, mcpVal, akyVal, traVal;

uint8_t segPos[3], segDP[3], segCP;


void setup(void)
{
    // Boot time check
    i2cInit();
    bootTime = bootCheck();

    // LCD init.
    lcdInit(lcdDispFR_90);
    lcdSetFrontColor(0xffff), lcdSetBackColor(0x0000);
    lcdClearScreen();

    // LED init.
    ledUpdate(bootTime);

    // Seg. init.
    segInit();
    segSend(seg7Mask[bootTime / 100], seg7Mask[(bootTime / 10) % 10], seg7Mask[bootTime % 10]);

    // Display boot info.
    //printf("\n\n",dispBuffer);
    std::cout<<"\n\n";
  
    sprintf(dispBuffer, "CT117E-M4  Boot=%u", bootTime);
    lcdDisplayString(Line0, dispBuffer);
    //printf("%s\n",dispBuffer);
    std::cout<<dispBuffer<<'\n';

    sprintf(dispBuffer, "Compiled in");
    lcdDisplayString(Line1, dispBuffer);
    //printf("%s\n",dispBuffer);
    std::cout<<dispBuffer<<'\n';
    
    sprintf(dispBuffer, "%s,%s", __TIME__, __DATE__);
    lcdDisplayString(Line2, dispBuffer);
    //printf("%s\n",dispBuffer);
    std::cout<<dispBuffer<<'\n';

    HAL_Delay(1500);

    // ADC start
    adcConvStart();

    // UART start, DMA2CH1
    if (HAL_UART_Receive_DMA(&huart1, uartBuffer, sizeof(uartBuffer)) != HAL_OK)
        Error_Handler();
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    // R40 InputCapture start
    if (HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1) != HAL_OK)
        Error_Handler();

    // R39 InputCapture start
    if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1) != HAL_OK)
        Error_Handler();

    // Clear display
    segSend(seg7Mask[16], seg7Mask[16], seg7Mask[16]);
    lcdClearScreen();
    ledUpdate(0);
    sTimerTS_lcdRefresh = HAL_GetTick();
}

void loop(void)
{
    uint8_t errTime;
    uint32_t nowTick;
    // Check ADC Conv. Value
    adcConvCheck(&r37Val, &r38Val, &mcpVal, &akyVal, &traVal);

    // Key Scan & AKEY Conv.
    keyScan();
    akyNum = akyConv(akyVal);

    // MCP4017 Read
    MCP_CFGVal = mcpRead();
#ifdef CT117EXA
    // DHT11 Read
    nowTick = HAL_GetTick();
    if (nowTick - sTimerTS_dht11Refresh > 2000)
    {
        sTimerTS_dht11Refresh = nowTick;
        errTime = 5;
        while (DHT11_ReadData(&DHT11_Humi, &DHT11_Temp) || !errTime)
            errTime--;
        if (!errTime)
            Error_Handler();
    }

    // DS18B20 Read
    nowTick = HAL_GetTick();
    if (nowTick - sTimerTS_ds18b20Update > 750)
    {
        sTimerTS_ds18b20Update = nowTick;
        errTime = 5;
        if (DS18B20_Mode)
        {
            while (DS18B20_Start(DS18B20_Res) || !errTime)
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

    // MCP4017 Write
    mcpWrite(MCP_CFGVal);

    // LED Display
    ledUpdate(ledBuffer);

    // Seg Display
    segSend((seg7Mask[segPos[0]] | segDP[0]), (seg7Mask[segPos[1]] | segDP[1]), (seg7Mask[segPos[2]] | segDP[2]));

    // LCD Display
    nowTick = HAL_GetTick();
    if (nowTick - sTimerTS_lcdRefresh > 100)
    {
        sTimerTS_lcdRefresh = nowTick;

        printf("\n");

        lcdClearLine(Line0);
        sprintf(dispBuffer, "S=%s,K=%s", sModeText, kModeText);
        lcdDisplayString(Line0, dispBuffer), printf("%s\n", dispBuffer);
        switch (sMode)
        {
        case sMode_M4:
            lcdClearLine(Line1);
            sprintf(dispBuffer, "A1=%1.2fV,F1=%uHz", r37Val, r39Freq);
            lcdDisplayString(Line1, dispBuffer), printf("%s\n", dispBuffer);

            lcdClearLine(Line2);
            sprintf(dispBuffer, "A2=%1.2fV,F2=%uHz", r38Val, r40Freq);
            lcdDisplayString(Line2, dispBuffer), printf("%s\n", dispBuffer);

            lcdClearLine(Line3);
            sprintf(dispBuffer, "Res=%1.2fV,%.2fKOhm", mcpVal, MCP_CFGVal * 0.7874);
            lcdDisplayString(Line3, dispBuffer), printf("%s\n", dispBuffer);

            break;
        case sMode_EXA:
            lcdClearLine(Line1);
            sprintf(dispBuffer, "AKY=%1.2fV,%u", akyVal, akyNum);
            lcdDisplayString(Line1, dispBuffer), printf("%s\n", dispBuffer);

            lcdClearLine(Line2);
            sprintf(dispBuffer, "TR=%1.2fV,DS=%2.1f,%ub", traVal, DS18B20_Temp, DS18B20_Res);
            lcdDisplayString(Line2, dispBuffer), printf("%s\n", dispBuffer);

            lcdClearLine(Line3);
            sprintf(dispBuffer, "DHT=%2.1f,%2.1f%%", DHT11_Temp, DHT11_Humi);
            lcdDisplayString(Line3, dispBuffer), printf("%s\n", dispBuffer);

            lcdClearLine(Line4);
            sprintf(dispBuffer, "SEG=%x%s%x%s%x%s,CHG=%u", segPos[0], (segDP[0] ? "." : ""), segPos[1],
                    (segDP[1] ? "." : ""), segPos[2], (segDP[2] ? "." : ""), (segCP + 1));
            lcdDisplayString(Line4, dispBuffer), printf("%s\n", dispBuffer);

            break;
        default:
            break;
        }
    }
}

void keyProg(void)
{
    char modeDbuf[16];

    /* B1 Short */
    if (keyState[0] == S2)
    {
        // reset state
        keyState[0] = S0;

        // act.
        switch (kMode)
        {
        case kMode_LED:
            kMode = kMode_MCP;
            sprintf(kModeText, "MCP");
            break;
        case kMode_MCP:
            kMode = kMode_DS;
            sprintf(kModeText, "DSR");
            break;
        case kMode_DS:
            kMode = kMode_SEG;
            sprintf(kModeText, "SEG");
            break;
        case kMode_SEG:
            kMode = kMode_LED;
            sprintf(kModeText, "LED");
            break;
        default:
            kMode = kMode;
            break;
        }

        // disp.
        lcdClearLine(Line9);
        sprintf(dispBuffer, "K%s,%s", "1S", "keyMode Change");
        lcdDisplayString(Line9, dispBuffer);
    }

    /* B1 Long */
    if (keyState[0] == S4)
    {
        // reset state
        keyState[0] = S0;

        // act.
        switch (sMode)
        {
        case sMode_M4:
            sMode = sMode_EXA;
            sprintf(sModeText, "EXA");
            lcdClearScreen();
            sTimerTS_lcdRefresh = HAL_GetTick();
            break;
        case sMode_EXA:
            sMode = sMode_M4;
            sprintf(sModeText, "M4");
            lcdClearScreen();
            sTimerTS_lcdRefresh = HAL_GetTick();
            break;
        default:
            sMode = sMode;
            break;
        }

        // disp.
        lcdClearLine(Line9);
        sprintf(dispBuffer, "K%s,%s", "1L", "sysMode Change");
        lcdDisplayString(Line9, dispBuffer);
    }

    /* B2 Short */
    if (keyState[1] == S2)
    {
        // reset state
        keyState[1] = S0;

        // act.
        switch (kMode)
        {
        default:
            sprintf(modeDbuf, "");
            break;
        }

        // disp.
        lcdClearLine(Line9);
        sprintf(dispBuffer, "K%s,%s", "2S", modeDbuf);
        lcdDisplayString(Line9, dispBuffer);
    }

    /* B2 Long */
    if (keyState[1] == S4)
    {
        // reset state
        keyState[1] = S0;

        // act.
        switch (kMode)
        {
        default:
            sprintf(modeDbuf, "");
            break;
        }

        // disp.
        lcdClearLine(Line9);
        sprintf(dispBuffer, "K%s,%s", "2L", modeDbuf);
        lcdDisplayString(Line9, dispBuffer);
    }

    /* B3 Short */
    if (keyState[2] == S2)
    {
        // reset state
        keyState[2] = S0;

        // act.
        switch (kMode)
        {
        case kMode_MCP:
            mcpWrite(--MCP_CFGVal);
            sprintf(modeDbuf, "Res -1");
            break;
        case kMode_LED:
            ledBuffer >>= 1;
            if (!ledBuffer)
                ledBuffer = 0x80;
            sprintf(modeDbuf, "LED >>");
            break;
        case kMode_DS:
            DS18B20_Res--;
            if (DS18B20_Res < 9)
                DS18B20_Res = 9;
            sprintf(modeDbuf, "DS_Res -1");
            break;
        case kMode_SEG:
            segPos[segCP]--;
            if (segPos[segCP] > 0xf)
                segPos[segCP] = 0xf;
            sprintf(modeDbuf, "SEG%u -1", segCP + 1);
            break;

        default:
            sprintf(modeDbuf, "");
            break;
        }

        // disp.
        lcdClearLine(Line9);
        sprintf(dispBuffer, "K%s,%s", "3S", modeDbuf);
        lcdDisplayString(Line9, dispBuffer);

        // act.
    }

    /* B3 Long */
    if (keyState[2] == S4)
    {
        // reset state
        keyState[2] = S0;

        // act.
        switch (kMode)
        {
        case kMode_MCP:
            MCP_CFGVal = 0;
            mcpWrite(MCP_CFGVal);
            sprintf(modeDbuf, "Res Min");
            break;
        case kMode_LED:
            ledBuffer = 0;
            sprintf(modeDbuf, "LED Blank");
            break;

        case kMode_DS:
            DS18B20_Res = 9;
            sprintf(modeDbuf, "DS_Res Min");
            break;

        case kMode_SEG:
            segCP++;
            if (segCP > 2)
                segCP = 0;
            sprintf(modeDbuf, "SEG ChangePos");
            break;

        default:
            sprintf(modeDbuf, "");
            break;
        }

        // disp.
        lcdClearLine(Line9);
        sprintf(dispBuffer, "K%s,%s", "3L", modeDbuf);
        lcdDisplayString(Line9, dispBuffer);
    }

    /* B4 Short */
    if (keyState[3] == S2)
    {
        // reset state
        keyState[3] = S0;

        // act.
        switch (kMode)
        {
        case kMode_MCP:
            mcpWrite(++MCP_CFGVal);
            sprintf(modeDbuf, "Res +1");
            break;
        case kMode_LED:
            ledBuffer <<= 1;
            if (!ledBuffer)
                ledBuffer = 1;
            sprintf(modeDbuf, "LED <<");
            break;
        case kMode_DS:
            DS18B20_Res++;
            if (DS18B20_Res > 12)
                DS18B20_Res = 12;
            sprintf(modeDbuf, "DS_Res +1");
            break;
        case kMode_SEG:
            segPos[segCP]++;
            if (segPos[segCP] > 0xf)
                segPos[segCP] = 0;
            sprintf(modeDbuf, "SEG%u +1", segCP + 1);
            break;

        default:
            sprintf(modeDbuf, "");
            break;
        }

        // disp.
        lcdClearLine(Line9);

        sprintf(dispBuffer, "K%s,%s", "4S", modeDbuf);
        lcdDisplayString(Line9, dispBuffer);
    }

    /* B4 Long */
    if (keyState[3] == S4)
    {
        // reset state
        keyState[3] = S0;

        // act.
        switch (kMode)
        {
        case kMode_MCP:
            MCP_CFGVal = 0x7f;
            mcpWrite(MCP_CFGVal);
            sprintf(modeDbuf, "Res Max");
            break;
        case kMode_LED:
            ledBuffer = 0xff;
            sprintf(modeDbuf, "LED White");
            break;
        case kMode_DS:
            DS18B20_Res = 12;
            sprintf(modeDbuf, "DS_Res Max");
            break;

        case kMode_SEG:
            segDP[segCP] = (segDP[segCP] ? 0 : 0x80);
            sprintf(modeDbuf, "SEG%u PT Change", segCP + 1);
            break;
        default:
            sprintf(modeDbuf, "");
            break;
        }

        // disp.
        lcdClearLine(Line9);
        sprintf(dispBuffer, "K%s,%s", "4L", modeDbuf);
        lcdDisplayString(Line9, dispBuffer);
    }
}
