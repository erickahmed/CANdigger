/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : canlog.h
  * @brief          : Header for canlog.c file.
  *                   This file contains the defines related to SAE J1939 CAN.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CANLOG_H
#define CANLOG_H

#include "main.h"

/* Decladerations for queues */
extern osMessageQueueId_t xCAN1RxQueue;
extern osMessageQueueId_t xCAN2RxQueue;

/* Decladerations for thread handles and attributes */
extern osThreadId_t vCAN1_rx;
extern osThreadId_t vCAN2_rx;
extern osThreadId_t vLED_CAN1_Heartbeat;
extern osThreadId_t vLED_CAN2_Heartbeat;

extern const osThreadAttr_t vCAN1_rx_attributes;
extern const osThreadAttr_t vCAN2_rx_attributes;
extern const osThreadAttr_t vLED_CAN1_Heartbeat_attributes;
extern const osThreadAttr_t vLED_CAN2_Heartbeat_attributes;

/**
  * @brief  Initializes the CAN logger modules, OS threads, queues, and hardware.
  * @param  hcan1 Pointer to the CAN1 handle
  * @param  hcan2 Pointer to the CAN2 handle
  */
void vCANLoggerInit(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void vCANLoggerListen(void *argument);
void vLEDHeartbeat(void *argument);

#endif /* CANLOG_H */
