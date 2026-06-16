/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : canlog.c
  * @brief          : SAE J1939 CAN Real Time listening and decoding
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 Erick Ahmed.
  *
  * SPDX-License-Identifier: GPL-3.0-or-later
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

extern osThreadId_t xLedTaskCAN1;
extern osThreadId_t xLedTaskCAN2;

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
    if (HAL_CAN_Start(hcan1) != HAL_OK) Error_Handler();

    filter.FilterBank = 14;
    if (HAL_CAN_ConfigFilter(hcan2, &filter) != HAL_OK) Error_Handler();
    if (HAL_CAN_Start(hcan2) != HAL_OK) Error_Handler();
}
/* END CAN_Logger_Init */

/* BEGIN HAL_CAN_RxFifo0MsgPendingCallback */
/**
  * @brief  ISR for CAN message pending in FIFO0
  * @param  argument: Can handle hcan (hcan1 or hcan2)
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* CODE BEGIN */
    CanMessage_t message;
    CAN_RxHeaderTypeDef rxHeader;
    uint8_t data[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, data) != HAL_OK) Error_Handler();

    message.id = rxHeader.ExtId;
    message.dlc = rxHeader.DLC;
    message.isExtended = (rxHeader.IDE == CAN_ID_EXT);
    memcpy(message.payload, data, rxHeader.DLC);

    osMessageQueueId_t queue = (hcan->Instance == CAN1) ? xCAN1RxQueue : xCAN2RxQueue;
    if (osMessageQueuePut(queue, &message, 0U, 0U) == osOK)
    {
      if (hcan->Instance == CAN1) osThreadFlagsSet(xLedTaskCAN1, 0x01);
      else osThreadFlagsSet(xLedTaskCAN2, 0x01);
    }
    else
    {
        // TODO: better handling: flash error_led on osTimeout, call Error_Handler() when other errors

        // send errors like queue full via UART
    }
    /* CODE END */
}
/* END HAL_CAN_RxFifo0MsgPendingCallback */

/* BEGIN vCANLoggerListen */
/**
  * @brief  Log incoming CAN bus traffic in FIFO buffer
  * @param  argument: Not used
  * @retval None
  */
void vCANLoggerListen(void *argument)
{
  /* CODE BEGIN */
  CAN_HandleTypeDef *hcan = (CAN_HandleTypeDef*)argument;
  osMessageQueueId_t queue = (hcan->Instance == CAN1) ? xCAN1RxQueue : xCAN2RxQueue;
  CanMessage_t message;

  HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

  for (;;)
  {
    if (osMessageQueueGet(queue, &message, NULL, osWaitForever) == osOK)
	{
					//
	}
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
    LEDContext *context = (LEDContext*)argument;

  for (;;)
  {
    uint32_t ret = osThreadFlagsWait(0x01, osFlagsWaitAny, 25U);

    if (ret & 0x01) HAL_GPIO_WritePin(context->led->port, context->led->pin, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(context->led->port, context->led->pin, GPIO_PIN_RESET);
  }
  /* CODE END */
}
/* END vLEDHeartbeat */
/* USER CODE END FunctionPrototypes */
