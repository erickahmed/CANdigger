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
#include "main.h"
#include "cmsis_os.h"
#include "canlog.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern osSemaphoreId_t xUARTDMASemaphore;
extern osMessageQueueId_t xUARTQueue;

/* BEGIN format_can_message */
/**
  * @brief  Format CAN message for friendly reading on terminal emulators and other MCUs.
  * @param  arguments: buffer, source, message
  * @retval None
  */
static int format_can_message(char *buf, uint8_t source, const CanMessage_t *message)
{
  const char hex[] = "0123456789ABCDEF";

  buf[0] = 'C';
  buf[1] = (source == 1) ? '1' : '2';
  buf[2] = ':';

  buf[3]  = hex[(message->id >> 28) & 0x0F];
  buf[4]  = hex[(message->id >> 24) & 0x0F];
  buf[5]  = hex[(message->id >> 20) & 0x0F];
  buf[6]  = hex[(message->id >> 16) & 0x0F];
  buf[7]  = hex[(message->id >> 12) & 0x0F];
  buf[8]  = hex[(message->id >> 8) & 0x0F];
  buf[9]  = hex[(message->id >> 4) & 0x0F];
  buf[10] = hex[message->id & 0x0F];

  buf[11] = ' ';
  buf[12] = hex[(message->dlc >> 4) & 0x0F];
  buf[13] = hex[message->dlc & 0x0F];
  buf[14] = ' ';

  int idx = 15;
  for (int i = 0; i < 8; i++)
  {
    if (i < message->dlc)
    {
      buf[idx++] = hex[(message->payload[i] >> 4) & 0x0F];
      buf[idx++] = hex[message->payload[i] & 0x0F];
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
  buf[idx] = '\0';

  return idx;
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
    osSemaphoreRelease(xUARTDMASemaphore);
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
  char tx_buffer[45];

  for (;;)
  {
    DEBUG_PRINT("Waiting for CAN traffic...\r\n");
    if (osMessageQueueGet(xUARTQueue, &message, NULL, osWaitForever) == osOK)
    {
      osSemaphoreAcquire(xUARTDMASemaphore, osWaitForever);
      int len = format_can_message(tx_buffer, message.source, &message);

      if (HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tx_buffer, len) == HAL_OK)
      {
        DEBUG_PRINT("CAN frame sent via UART\r\n");
      }
      else
      {
        osSemaphoreRelease(xUARTDMASemaphore);
        DEBUG_PRINT("HAL error, CAN frame NOT sent!\r\n");
      }
    }
  }
}
/* END vUARTLoggerListen */
