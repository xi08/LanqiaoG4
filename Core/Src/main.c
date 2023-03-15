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
#include "dma.h"
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
volatile uint8_t sysTime_msFlag;
volatile uint32_t sysTime = 0;

uint8_t bootTime[4] = {0};
const uint8_t bootTimePos[4] = {'S', 'u', 'e', 's'};

extern keyState_enum keyState[keyNum];
uint8_t ledBuffer = 0x1;
uint8_t dispBuffer[21];

uint16_t adc1Val[2], adc2Val, r39Val, r40Val;
float r37V, r38V, mcpV, mcpR;
uint8_t mcp_res;

uint8_t uart1Buffer[256];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void keyUpdate(void);
void keyResp(void);
void ledUpdate(uint8_t led);
void uartRespRX(void);
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
    uint8_t main_i = 0;
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    LCD_Init();
    LCD_Clear(White);

    I2CInit();
    for (main_i = 0; main_i < 4; main_i++)
    {
        bootTime[main_i] = eeRead(bootTimePos[main_i]);
        HAL_Delay(1);
        if (bootTime[main_i] != bootTime[0])
        {
            bootTime[0] = 0;
            break;
        }
    }
    bootTime[0]++;
    for (main_i = 0; main_i < 4; main_i++)
    {
        eeWrite(bootTimePos[main_i], bootTime[0]);
        HAL_Delay(1);
    }

    ledBuffer = eeRead(0x00);
    ledUpdate(ledBuffer);

    mcp_res = mcpRead();
    mcpR = mcp_res * 0.7874;

    sprintf((char *)dispBuffer, "%s  Boot=%u", "CT117E-M4", bootTime[0]);
    LCD_DisplayStringLine(Line0, dispBuffer);
    printf("%s\n", dispBuffer);
    LCD_DisplayStringLine(Line1, (uint8_t *)"Compiled in");
    sprintf((char *)dispBuffer, "%s,%s", __TIME__, __DATE__);
    LCD_DisplayStringLine(Line2, dispBuffer);
    printf("%s\n%s\n", "Compiled in", dispBuffer);
    HAL_Delay(1000);
    LCD_ClearLine(Line1);
    LCD_ClearLine(Line2);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc1Val, 2);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)&adc2Val, 1);

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1Buffer, (sizeof(uart1Buffer) - 1));

    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);

    printf("Init OK\n");

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        r37V = adc2Val * 3.3 / 4095;
        r38V = adc1Val[0] * 3.3 / 4095;
        mcpV = adc1Val[1] * 3.3 / 4095;

        if (sysTime_msFlag)
        {
            sysTime_msFlag = 0;
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
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        keyUpdate();
        keyResp();

        ledUpdate(ledBuffer);
        eeWrite(0x00, ledBuffer);

        mcp_res = mcpRead();
        mcpR = mcp_res * (7.874e-1);
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
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
        // act.
        ledBuffer <<= 1;
        if (ledBuffer == 0x0)
            ledBuffer = 0x01;

        keyState[0] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B1 Long");
        // act.
        ledBuffer = 0x00;

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
        // act.
        ledBuffer >>= 1;
        if (ledBuffer == 0x0)
            ledBuffer = 0x80;

        keyState[1] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B2 Long");

        // act.
        ledBuffer = 0xff;

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
        // act.
        mcpWrite(--mcp_res);

        keyState[2] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B3 Long");
        // act.
        mcp_res = 0;
        mcpWrite(mcp_res);

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
        // act.
        mcpWrite(++mcp_res);

        keyState[3] = S0; // reset state
        break;

    case S4: // Long
        LCD_ClearLine(Line7);
        LCD_DisplayStringLine(Line7, (uint8_t *)"B4 Long");
        // act.
        mcp_res = 0x7f;
        mcpWrite(mcp_res);

        keyState[3] = S0; // reset state
        break;

    default:
        break;
    }
}

void uartRespRX(void)
{
    LCD_ClearLine(Line8);
    LCD_DisplayStringLine(Line8, uart1Buffer);
    printf("%s", uart1Buffer);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    uint32_t timVal = 0;
    if (htim->Instance == TIM2)
    {
        timVal = __HAL_TIM_GET_COUNTER(&htim2);
        r40Val = 1000000 / timVal;
        __HAL_TIM_SetCounter(&htim2, 0);
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
    }
    if (htim->Instance == TIM3)
    {
        timVal = __HAL_TIM_GET_COUNTER(&htim3);
        r39Val = 1000000 / timVal;
        __HAL_TIM_SetCounter(&htim3, 0);
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
    }
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        uartRespRX();
        memset(uart1Buffer, 0, sizeof(uart1Buffer));
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1Buffer, (sizeof(uart1Buffer) - 1));
    }
}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
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
