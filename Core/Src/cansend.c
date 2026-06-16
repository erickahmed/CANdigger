/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : cansend.c
  * @brief          : Send logged SAE J1939 CAN via serial and save via SDIO.
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
#include "cansend.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
/* USER CODE END Includes */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void vUARTLogger(void *argument);
