/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : canlog.c
  * @brief          : SAE J1939 CAN Real Time listening and decoding
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "canlog.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include <string.h>
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* BEGIN CAN_Logger_Init */
/**
  * @brief  Initialize CAN bus logger.
  * @param  argument: hcan1, hcan2
  * @retval None
  */
void CAN_Logger_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2)
{
    CAN_FilterTypeDef filter = {0};
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterIdLow = 0x0000;
    filter.FilterMaskIdLow = 0x0000;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    filter.FilterBank = 0;
    if (HAL_CAN_ConfigFilter(hcan1, &filter) != HAL_OK) Error_Handler();
    filter.FilterBank = 14;
    if (HAL_CAN_ConfigFilter(hcan2, &filter) != HAL_OK) Error_Handler();

    if (HAL_CAN_Start(hcan1) != HAL_OK) Error_Handler();
    if (HAL_CAN_Start(hcan2) != HAL_OK) Error_Handler();

    if (HAL_CAN_ActivateNotification(hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) Error_Handler();
    if (HAL_CAN_ActivateNotification(hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) Error_Handler();
}
/* END CAN_Logger_Init */

/* BEGIN vCANLoggerListen */
/**
  * @brief  Log incoming CAN bus traffic in FIFO buffer
  * @param  argument: Not used
  * @retval None
  */
void vCANLoggerListen(void *argument)
{
    /* CODE BEGIN */
    /* Infinite loop */
    for(;;)
    {
      osDelay(1);
    }
    /* CODE END */
}
/* END vCANLoggerListen */

/* BEGIN vLEDHeartbeat */
/**
  * @brief  Blink a LED in heartbeat mode when CAN traffic is detected
  * @param  argument: LED GPIO (port and pin)
  * @retval None
  */
void vLEDHeartbeat(void *argument)
{
  /* CODE BEGIN */
  LED_Config *led = (LED_Config*)argument;
  HAL_GPIO_TogglePin(led->port, led->pin);
  /* CODE END */
}
/* END vLEDHeartbeat */
/* USER CODE END FunctionPrototypes */
