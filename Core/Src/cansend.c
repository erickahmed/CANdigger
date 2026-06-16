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

/* BEGIN format_can_message */
/**
  * @brief  Format CAN message for friendly reading on terminal emulators and other MCUs.
  * @param  arguments: buffer, sourse, message
  * @retval None
  */
static void format_can_message(char *buf, uint8_t source, const CanMessage_t *msg)
{
  const char hex[] = "0123456789ABCDEF";

  buf[0] = 'C';
  buf[1] = (source == 1) ? '1' : '2';
  buf[2] = ':';

  buf[3]  = hex[(msg->id >> 28) & 0x0F];
  buf[4]  = hex[(msg->id >> 24) & 0x0F];
  buf[5]  = hex[(msg->id >> 20) & 0x0F];
  buf[6]  = hex[(msg->id >> 16) & 0x0F];
  buf[7]  = hex[(msg->id >> 12) & 0x0F];
  buf[8]  = hex[(msg->id >> 8) & 0x0F];
  buf[9]  = hex[(msg->id >> 4) & 0x0F];
  buf[10] = hex[msg->id & 0x0F];

  buf[11] = ' ';
  buf[12] = hex[(msg->dlc >> 4) & 0x0F];
  buf[13] = hex[msg->dlc & 0x0F];
  buf[14] = ' ';

  int idx = 15;
  for (int i = 0; i < 8; i++)
  {
    if (i < msg->dlc)
    {
      buf[idx++] = hex[(msg->payload[i] >> 4) & 0x0F];
      buf[idx++] = hex[msg->payload[i] & 0x0F];
    }
    else
    {
      buf[idx++] = ' ';
      buf[idx++] = ' ';
    }
    if (i < 7) buf[idx++] = ' ';
  }

  buf[idx++] = '\r';
  buf[idx++] = '\n';
}
/* END format_can_message */

/* BEGIN HAL_UART_TxCpltCallback */
/**
  * @brief  Safely release UART semaphore inside ISR and yield task if possible.
  * @param  argument: UART handle
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    osSemaphoreReleaseFromISR(xUartDmaSem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}
/* END HAL_UART_TxCpltCallback */

/* BEGIN vUARTLoggerListen */
/**
  * @brief  Send CAN bus traffic saved in FIFO buffer to USART1
  * @param  argument: Not used
  * @retval None
  */
void vUARTLogger(void *argument)
{
  CanMessage_t message;
  char tx_buffer[45]; // converted extended frame
  bool queues_empty;

  for (;;)
  {
    osEventFlagsWait(xCanEventFlags, 0x03, osFlagsWaitAny, osWaitForever);

    do
    {
      queues_empty = true;

      if (osMessageQueueGet(xCAN1RxQueue, &message, NULL, 0U) == osOK)
      {
        format_can_message(tx_buffer, 1, &message);

        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tx_buffer, 44);
        osSemaphoreAcquire(xUartDmaSem, osWaitForever);

        quesues_empty = false;
      }
    }
    while (!queues_empty);
  }
}
/* END vUARTLoggerListen */
