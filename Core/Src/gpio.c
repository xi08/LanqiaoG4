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

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);

    /**/
    LL_GPIO_ResetOutputPin(LD6_GPIO_Port, LD6_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LD7_GPIO_Port, LD7_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LD8_GPIO_Port, LD8_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LD1_GPIO_Port, LD1_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LD3_GPIO_Port, LD3_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LD4_GPIO_Port, LD4_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LD5_GPIO_Port, LD5_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LE_GPIO_Port, LE_Pin);

    /**/
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);

    /**/
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    /**/
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin);

    /**/
    LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin);

    /**/
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);

    /**/
    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);

    /**/
    GPIO_InitStruct.Pin = LD6_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD6_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LD7_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD7_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LD8_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD8_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = B4_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(B4_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = B2_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(B2_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = B3_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(B3_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LD1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD1_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = nRD_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(nRD_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LD3_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LD4_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LD5_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LD5_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LE_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LE_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = nWR_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(nWR_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = SCL_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = SDA_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = RS_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(RS_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = nCS_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(nCS_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 2 */

void keyUpdate(void)
{
    uint8_t i = keyNum, keyInfo = 0xff;

    keyInfo ^= ((LL_GPIO_IsInputPinSet(B1_GPIO_Port, B1_Pin)) << 0);
    keyInfo ^= ((LL_GPIO_IsInputPinSet(B2_GPIO_Port, B2_Pin)) << 1);
    keyInfo ^= ((LL_GPIO_IsInputPinSet(B3_GPIO_Port, B3_Pin)) << 2);
    keyInfo ^= ((LL_GPIO_IsInputPinSet(B4_GPIO_Port, B4_Pin)) << 3);

    while (i--)
    {
        /* Pressed */
        if ((keyInfo & (1 << i)) == 1)
        {
            switch (keyState[i])
            {
            case S0:
                keyState[i] = S2;          // switch state
                keyUpdate_TS[i] = sysTime; // update timestamp
                break;

            default:
                break;
            }
        }
        /* Not Pressed */
        if ((keyInfo & (1 << i)) == 0)
        {
            switch (keyState[i])
            {
            case S2:
                if (sysTime - keyUpdate_TS[i] >= keyLongPressTime) // S3 detection
                    keyState[i] = S3;
                else if (sysTime - keyUpdate_TS[i] >= keyShortPressTime) // S1 detection
                    keyState[i] = S1;
                else
                    keyState[i] = S0; // reset state
                break;

            default:
                keyState[i] = S0; // reset state
                break;
            }
        }
    }
}

void ledUpdate(uint8_t _led)
{
    LL_GPIO_WriteOutputPort(LD1_GPIO_Port, ~_led << 8);
    LL_GPIO_SetOutputPin(LE_GPIO_Port, LE_Pin);
    LL_GPIO_ResetOutputPin(LE_GPIO_Port, LE_Pin);
}

/* USER CODE END 2 */
