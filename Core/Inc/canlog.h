#ifndef CANLOG_H
#define CANLOG_H

#include "main.h"

/**
  * @brief  Initializes the CAN logger modules, OS threads, queues, and hardware.
  * @param  hcan1 Pointer to the CAN1 handle
  * @param  hcan2 Pointer to the CAN2 handle
  */
void CAN_Logger_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2);
void vCAN_log(void *argument);
void vLED_HeartbeatOnCanRx(void *argument);

#endif /* CANLOG_H */
