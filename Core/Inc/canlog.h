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
void CAN_Logger_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2);
void vCAN_Logger_Listen(void *argument);
void vLED_Heartbeat(void *argument);

#endif /* CANLOG_H */
