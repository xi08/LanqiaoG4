/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ee_mcp.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

volatile uint32_t sysTime = 0;
volatile uint8_t sysTime_msFlag = 0;

extern keyState_enum keyState[keyNum];

uint8_t ledBuffer = 0x0;
uint8_t dispBuffer[21];

char uartRxBuffer[uartBufferSize];
uint8_t uartRxBufferIdx = 0;
uint8_t uartRxOKFlag = 0;

float r37V, r38V, mcpV, mcpR;
uint8_t mcp_res;
uint16_t r39Val, r40Val;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void keyUpdate(void);
void keyResp(void);
void ledUpdate(uint8_t _led);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

    /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
     */
    LL_PWR_DisableUCPDDeadBattery();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    MX_TIM7_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_TIM17_Init();
    MX_DAC1_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    I2CInit();
    LCD_Init();
    LCD_Clear(White);
    ledUpdate(ledBuffer);

    LCD_DisplayStringLine(Line0, (uint8_t *)"CT117E-M4");
    LCD_DisplayStringLine(Line1, (uint8_t *)"Compiled in");
    sprintf((char *)dispBuffer, "%s,%s", __TIME__, __DATE__);
    LCD_DisplayStringLine(Line2, dispBuffer);
    printf("%s\n%s\n%s\n", "CT117E-M4", "Compiled in", dispBuffer);
    LL_mDelay(1000);
    LCD_ClearLine(Line1);
    LCD_ClearLine(Line2);
    printf("Init OK\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        if (sysTime_msFlag & sysTime_msFlag_5ms)
        {
            sysTime_msFlag &= ~sysTime_msFlag_5ms;
        }

        if (sysTime_msFlag & sysTime_msFlag_10ms)
        {
            sysTime_msFlag &= ~sysTime_msFlag_10ms;
        }
        if (sysTime_msFlag & sysTime_msFlag_100ms)
        {
            sysTime_msFlag &= ~sysTime_msFlag_100ms;
        }

        if (sysTime_msFlag & sysTime_msFlag_1000ms)
        {
            sysTime_msFlag &= ~sysTime_msFlag_1000ms;

            adc_getVal();

            LCD_ClearLine(Line1);
            sprintf((char *)dispBuffer, "R37=%.2fV", r37V);
            LCD_DisplayStringLine(Line1, dispBuffer);

            LCD_ClearLine(Line2);
            sprintf((char *)dispBuffer, "R38=%.2fV", r38V);
            LCD_DisplayStringLine(Line2, dispBuffer);

            LCD_ClearLine(Line3);
            sprintf((char *)dispBuffer, "MCP=%.2fV", mcpV);
            LCD_DisplayStringLine(Line3, dispBuffer);

            LCD_ClearLine(Line4);
            sprintf((char *)dispBuffer, "MCP=%.2fK", mcpR);
            LCD_DisplayStringLine(Line4, dispBuffer);

            LCD_ClearLine(Line5);
            sprintf((char *)dispBuffer, "R39=%uHz", r39Val);
            LCD_DisplayStringLine(Line5, dispBuffer);

            LCD_ClearLine(Line6);
            sprintf((char *)dispBuffer, "R40=%uHz", r40Val);
            LCD_DisplayStringLine(Line6, dispBuffer);

            printf("\n\nR37=%.2fV,R38=%.2fV\n", r37V, r38V);
            printf("R39=%uHz,R40=%uHz\n", r39Val, r40Val);
            printf("MCP=%.2fV,%.2fK\n", mcpV, mcpR);
        }
        if (uartRxOKFlag)
        {
            uartRxOKFlag = 0;
            printf("%s", uartRxBuffer);
        }

        keyUpdate();
        keyResp();
        ledUpdate(ledBuffer);

        mcp_res = mcpRead();
        mcpR = mcp_res * (7.874e-1);

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
    {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSE_Enable();
    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1)
    {
    }

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_3, 20, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_Enable();
    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1)
    {
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_Init1msTick(80000000);

    LL_SetSystemCoreClock(80000000);
}

/* USER CODE BEGIN 4 */

void keyResp(void)
{
    /* B1 */
    switch (keyState[0])
    {
    case S3: // Short

        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B1 Short");
        ledBuffer <<= 1;
        if (ledBuffer == 0x0)
            ledBuffer = 0x01;

        keyState[0] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B1 Long");
        keyState[0] = S0; // reset state
        break;

    default:
        break;
    }

    /* B2 */
    switch (keyState[1])
    {
    case S3: // Short
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B2 Short");
        ledBuffer >>= 1;
        if (ledBuffer == 0x0)
            ledBuffer = 0x80;

        keyState[1] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B2 Long");
        keyState[1] = S0; // reset state
        break;

    default:
        break;
    }

    /* B3 */
    switch (keyState[2])
    {
    case S3: // Short
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B3 Short");
        mcpWrite(--mcp_res);
        keyState[2] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B3 Long");
        keyState[2] = S0; // reset state
        break;

    default:
        break;
    }

    /* B4 */
    switch (keyState[3])
    {
    case S3: // Short
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B4 Short");
        mcpWrite(++mcp_res);
        keyState[3] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B4 Long");
        keyState[3] = S0; // reset state
        break;

    default:
        break;
    }
}

void uart_ReceiveIRQ(void)
{
    uint8_t ch = LL_USART_ReceiveData8(USART1);

    if (ch == (uint8_t)('\r'))
    {
        uartRxBuffer[uartRxBufferIdx] = 0;
        uartRxBufferIdx = 0;
        uartRxOKFlag = 1;
    }
    else
        uartRxBuffer[uartRxBufferIdx++] = ch;
}

void adc_getVal(void)
{
    LL_ADC_REG_StartConversion(ADC2);
    while (!LL_ADC_IsActiveFlag_EOC(ADC2))
        ;
    r37V = LL_ADC_REG_ReadConversionData12(ADC2) * 3.3 / 4095;
    LL_ADC_REG_StartConversion(ADC1);
    while (!LL_ADC_IsActiveFlag_EOC(ADC1))
        ;
    r38V = LL_ADC_REG_ReadConversionData12(ADC1) * 3.3 / 4095;
    while (!LL_ADC_IsActiveFlag_EOC(ADC1))
        ;
    mcpV = LL_ADC_REG_ReadConversionData12(ADC1) * 3.3 / 4095;
}

inline void msDelay(uint32_t t)
{
    LL_mDelay(t);
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
