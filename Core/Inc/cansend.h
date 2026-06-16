/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CANSEND.h
  * @brief          : Header for cansend.c file.
  *                   This file contains the defines related to CAN data sent
  *                   via UART/USART and save via .
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
#ifndef CANSEND_H
#define CANSEND_H

#include "main.h"

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void vUARTLogger(void *argument);

#endif /* CANSEND_H */
