/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */
uint32_t keyUpdate_TS[keyNum];
keyState_enum keyState[keyNum];
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LD6_Pin|LD7_Pin|LD8_Pin|LD1_Pin
                          |LD2_Pin|LD3_Pin|LD4_Pin|LD5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LE_GPIO_Port, LE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCL_Pin|SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = LD6_Pin|LD7_Pin|LD8_Pin|LD1_Pin
                          |LD2_Pin|LD3_Pin|LD4_Pin|LD5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = B4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = B1_Pin|B2_Pin|B3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void keyUpdate(void)
{
    uint8_t i = keyNum, keyInfo = 0xff;

    keyInfo ^= ((HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) << 0);
    keyInfo ^= ((HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin)) << 1);
    keyInfo ^= ((HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin)) << 2);
    keyInfo ^= ((HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin)) << 3);

    while (i--)
    {
        if ((keyInfo & (1 << i))) /* KeyDown */
        {
            switch (keyState[i])
            {
            /* Initial State */
            case S0:
                keyState[i] = S1;                // update to KeyDown
                keyUpdate_TS[i] = HAL_GetTick(); // set time counter
                break;

            default:
                break;
            }
        }

        else if (!(keyInfo & (1 << i))) /* KeyUp */
        {
            switch (keyState[i])
            {
            /* KeyDown State */
            case S1:
                if (HAL_GetTick() - (keyUpdate_TS[i]) >= keyLongPressTime)       // check long press
                    keyState[i] = S4;                                            // update to Long
                else if (HAL_GetTick() - (keyUpdate_TS[i]) >= keyShortPressTime) // check short press
                    keyState[i] = S3;                                            // update to Short
                else                                                             // jitter
                    keyState[i] = S0;                                            // clear state
                break;

            default:
                keyState[i] = S0; // clear state
                break;
            }
        }
    }
}

void ledUpdate(uint8_t led)
{
    (LD1_GPIO_Port->ODR)= (~led << 8);
    HAL_GPIO_WritePin(LE_GPIO_Port, LE_Pin, GPIO_PIN_SET); // stop latch
    nop3();
    HAL_GPIO_WritePin(LE_GPIO_Port, LE_Pin, GPIO_PIN_RESET); // start latch
    nop3();
}
/* USER CODE END 2 */
