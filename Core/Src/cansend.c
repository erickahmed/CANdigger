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
#include <stdio.h>
#include <stdarg.h>
#include "cmsis_os.h"
#include "stm32f4xx.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern osSemaphoreId_t xUARTDMASemaphore;
extern osMessageQueueId_t xUARTQueue;
extern UART_HandleTypeDef huart1;
extern osSemaphoreId_t xUARTDMASemaphore;

/* BEGIN uart_printf */
/**
  * @brief  Implementation of a simple print to UART.
  * @param  arguments: strings to print
  * @retval None
  */
void uart_printf(const char *fmt, ...)
{
  static char buf[128];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  if (len > 0)
  {
    if (osKernelGetState() == osKernelRunning && __get_IPSR() == 0)
    {
      if (osSemaphoreAcquire(xUARTDMASemaphore, 100U) == osOK)
      {
        HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
        osSemaphoreRelease(xUARTDMASemaphore);
      }
    }
    else
    {
      HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
    }
  }
}
/* END uart_printf */

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
  if (huart->Instance == USART1) osSemaphoreRelease(xUARTDMASemaphore);
}
/* END HAL_UART_TxCpltCallback */

/* BEGIN HAL_UART_ErrorCallback */
/**
  * @brief  Handle UART errors
  * @param  argument: UART handle
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  Error_Handler();
}
/* END HAL_UART_ErrorCallback */

/* BEGIN vUARTLogger */
/**
  * @brief  Send CAN bus traffic saved in FIFO buffer to USART1
  * @param  argument: Not used
  * @retval None
  */
void vUARTLogger(void *argument)
{
  CanMessage_t message;
  char tx_buffer[45];

  #ifdef DEBUG_DUMMY_FRAME
  CanMessage_t dummy_frame = {
    .id = 0x0CF00400,       // J1939 EEC1 ID
    .dlc = 8,               // 8 bytes of data
    .isExtended = 1,        // 29-bit Extended ID
    .source = 1,            // CAN1
    .payload = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22}
  };
  #endif

  for (;;)
  {
	uint32_t queue_timeout = 2500U;

	//#ifdef DEBUG_DUMMY_FRAME
    //uint32_t queue_timeout = 100U;
    //osMessageQueuePut(xUARTQueue, &dummy_frame, 0U, 0U);
    //osDelay(500);
    //#else
    //uint32_t queue_timeout = osWaitForever;
   // #endif

   	if (osMessageQueueGet(xUARTQueue, &message, NULL, queue_timeout) == osOK)
	{
      if (osSemaphoreAcquire(xUARTDMASemaphore, queue_timeout) == osOK)
      {
        int len = format_can_message(tx_buffer, message.source, &message);

        if (HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tx_buffer, len) != HAL_OK)
        {
          osSemaphoreRelease(xUARTDMASemaphore);
          DEBUG_PRINT("[E] HAL error, CAN frame NOT sent!\r\n");
        }
      }
      else DEBUG_PRINT("[E] Semaphore timeout! UART might be stuck in BUSY state.\r\n");
	}
   	else DEBUG_PRINT("[D] Waiting for CAN traffic...\r\n");
  }
}
/* END vUARTLogger */
