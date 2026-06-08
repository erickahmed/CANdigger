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
void vCAN_log(void *argument);
void vLED_HeartbeatOnCanRx(void *argument);

#endif /* CANLOG_H */
