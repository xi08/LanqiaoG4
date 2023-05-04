/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g4xx.h"
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum
{
    S0, // Empty
    S1, // Press
    S2, // Short
    S4, // Long
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
void keyProg(void);
void setup(void);
void loop(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD5_Pin LL_GPIO_PIN_13
#define LD5_GPIO_Port GPIOC
#define LD6_Pin LL_GPIO_PIN_14
#define LD6_GPIO_Port GPIOC
#define LD7_Pin LL_GPIO_PIN_15
#define LD7_GPIO_Port GPIOC
#define B4_Pin LL_GPIO_PIN_0
#define B4_GPIO_Port GPIOA
#define SER_Pin LL_GPIO_PIN_1
#define SER_GPIO_Port GPIOA
#define RCK_Pin LL_GPIO_PIN_2
#define RCK_GPIO_Port GPIOA
#define SCK_Pin LL_GPIO_PIN_3
#define SCK_GPIO_Port GPIOA
#define TRA_Pin LL_GPIO_PIN_4
#define TRA_GPIO_Port GPIOA
#define AKY_Pin LL_GPIO_PIN_5
#define AKY_GPIO_Port GPIOA
#define OWD_Pin LL_GPIO_PIN_6
#define OWD_GPIO_Port GPIOA
#define DHT_Pin LL_GPIO_PIN_7
#define DHT_GPIO_Port GPIOA
#define B1_Pin LL_GPIO_PIN_0
#define B1_GPIO_Port GPIOB
#define B2_Pin LL_GPIO_PIN_1
#define B2_GPIO_Port GPIOB
#define B3_Pin LL_GPIO_PIN_2
#define B3_GPIO_Port GPIOB
#define R38_Pin LL_GPIO_PIN_12
#define R38_GPIO_Port GPIOB
#define MCP_Pin LL_GPIO_PIN_14
#define MCP_GPIO_Port GPIOB
#define R37_Pin LL_GPIO_PIN_15
#define R37_GPIO_Port GPIOB
#define LD0_Pin LL_GPIO_PIN_8
#define LD0_GPIO_Port GPIOC
#define LD1_Pin LL_GPIO_PIN_9
#define LD1_GPIO_Port GPIOC
#define TXD_Pin LL_GPIO_PIN_9
#define TXD_GPIO_Port GPIOA
#define RXD_Pin LL_GPIO_PIN_10
#define RXD_GPIO_Port GPIOA
#define R40_Pin LL_GPIO_PIN_15
#define R40_GPIO_Port GPIOA
#define LD2_Pin LL_GPIO_PIN_10
#define LD2_GPIO_Port GPIOC
#define LD3_Pin LL_GPIO_PIN_11
#define LD3_GPIO_Port GPIOC
#define LD4_Pin LL_GPIO_PIN_12
#define LD4_GPIO_Port GPIOC
#define LE_Pin LL_GPIO_PIN_2
#define LE_GPIO_Port GPIOD
#define R39_Pin LL_GPIO_PIN_4
#define R39_GPIO_Port GPIOB
#define SCL_Pin LL_GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin LL_GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
