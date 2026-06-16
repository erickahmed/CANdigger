/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : canlog.h
  * @brief          : Header for canlog.c file.
  *                   This file contains the defines related to SAE J1939 CAN.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CANLOG_H
#define CANLOG_H

#include "main.h"
#include "cmsis_os.h"

extern osMessageQueueId_t xCAN1RxQueue;
extern osMessageQueueId_t xCAN2RxQueue;
extern osSemaphoreId_t xSemaphoreCAN1;
extern osSemaphoreId_t xSemaphoreCAN2;

/* USER CODE BEGIN PTD */
typedef struct {
    GPIO_TypeDef* port;
    uint16_t       pin;
} LED_Config;

typedef struct {
    uint32_t id;
    uint8_t  payload[8];
    uint8_t  dlc;
    uint8_t  isExtended;
    //uint32_t timestamp; //Enable TIM2: 42 MHz / (41+1) = 1 MHz → 1 µs tick; 32bit autoreload freerunning
    // this is for getting timestamps on can messages
} CanMessage_t;

typedef struct {
    LED_Config *led;
} LEDContext;
/* USER CODE END PTD */

/**
  * @brief  Initializes the CAN logger modules, OS threads, queues, and hardware.
  * @param  hcan1 Pointer to the CAN1 handle
  * @param  hcan2 Pointer to the CAN2 handle
  */
void CAN_Logger_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void vCANLoggerListen(void *argument);
void vLEDHeartbeat(void *argument);

#endif /* CANLOG_H */
