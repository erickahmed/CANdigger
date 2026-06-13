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

/* USER CODE BEGIN RTOS_TASKS */
/* Definitions for CAN1rx incoming */
osThreadId_t vCAN1_rx;
const osThreadAttr_t vCAN1_rx_attributes = {
  .name = "vCAN1_rx",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime1,
};

/* Definitions for CAN2rx incoming*/
osThreadId_t vCAN2_rx;
const osThreadAttr_t vCAN2_rx_attributes = {
  .name = "vCAN2_rx",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* USER END RTOS_TASKS */


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
}
/* END CAN_Logger_Init */

/* BEGIN vCAN_Logger_Listen */
/**
  * @brief  Log incoming CAN bus traffic in FIFO buffer
  * @param  argument: Not used
  * @retval None
  */
void vCAN_Logger_Listen(void *argument)
{
    /* CODE BEGIN */
    /* Infinite loop */
    for(;;)
    {
      osDelay(1);
    }
    /* CODE END */
}
/* END vCAN_Logger_Listen */

/* BEGIN vLED_Heartbeat */
/**
  * @brief  Blink a LED in heartbeat mode when CAN traffic is detected
  * @param  argument: LED GPIO (port and pin)
  * @retval None
  */
void vLED_Heartbeat(void *argument)
{
  /* CODE BEGIN */
  LED_Config *led = (LED_Config*)argument;
  HAL_GPIO_TogglePin(led->port, led->pin);
  /* CODE END */
}
/* END vLED_HeartbeatOnCanRx */
/* USER CODE END FunctionPrototypes */
