/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum 
{
    S0, /* Initial */
    S1, /* KeyDown */
    S3, /* Short */
    S4, /* Long */
} keyState_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern volatile uint8_t sysTime_msFlag;
extern volatile uint32_t sysTime;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD6_Pin LL_GPIO_PIN_13
#define LD6_GPIO_Port GPIOC
#define LD7_Pin LL_GPIO_PIN_14
#define LD7_GPIO_Port GPIOC
#define LD8_Pin LL_GPIO_PIN_15
#define LD8_GPIO_Port GPIOC
#define B4_Pin LL_GPIO_PIN_0
#define B4_GPIO_Port GPIOA
#define PWM1_Pin LL_GPIO_PIN_1
#define PWM1_GPIO_Port GPIOA
#define B1_Pin LL_GPIO_PIN_0
#define B1_GPIO_Port GPIOB
#define B2_Pin LL_GPIO_PIN_1
#define B2_GPIO_Port GPIOB
#define B3_Pin LL_GPIO_PIN_2
#define B3_GPIO_Port GPIOB
#define R38_Pin LL_GPIO_PIN_12
#define R38_GPIO_Port GPIOB
#define MCP407_Pin LL_GPIO_PIN_14
#define MCP407_GPIO_Port GPIOB
#define R37_Pin LL_GPIO_PIN_15
#define R37_GPIO_Port GPIOB
#define LD1_Pin LL_GPIO_PIN_8
#define LD1_GPIO_Port GPIOC
#define LD2_Pin LL_GPIO_PIN_9
#define LD2_GPIO_Port GPIOC
#define TXD_Pin LL_GPIO_PIN_9
#define TXD_GPIO_Port GPIOA
#define RXD_Pin LL_GPIO_PIN_10
#define RXD_GPIO_Port GPIOA
#define R40_Pin LL_GPIO_PIN_15
#define R40_GPIO_Port GPIOA
#define LD3_Pin LL_GPIO_PIN_10
#define LD3_GPIO_Port GPIOC
#define LD4_Pin LL_GPIO_PIN_11
#define LD4_GPIO_Port GPIOC
#define LD5_Pin LL_GPIO_PIN_12
#define LD5_GPIO_Port GPIOC
#define LE_Pin LL_GPIO_PIN_2
#define LE_GPIO_Port GPIOD
#define R39_Pin LL_GPIO_PIN_4
#define R39_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define keyNum 4 /* number of keys */
#define keyLongPressTime 800 /* time of long press in ms */
#define keyShortPressTime 50 /* time of short press in ms */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
