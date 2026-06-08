#include "canlog.h"
#include "cmsis_os.h"
#include <string.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/* FreeRTOS task definitions */

/* Definitions for CAN1rx incoming */
osThreadId_t vCAN1_rx;
const osThreadAttr_t vCAN1_rx_attributes = {
  .name = "vCAN1_rx",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime1,
};

/* Definitions for CAN2rx incoming*/
osThreadId_t vCAN2_rx;
const osThreadAttr_t vCAN2_rx_attributes = {
  .name = "vCAN2_rx",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};

/* Definitions for CAN1 LED heartbeat */
osThreadId_t vLED_CAN1_Heartbeat;
const osThreadAttr_t vLED_CAN1_Heartbeat_attributes = {
  .name = "vLED_CAN1_HeartbeatTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};

/* Definitions for CAN2 LED heartbeat */
osThreadId_t vLED_CAN2_Heartbeat;
const osThreadAttr_t vLED_CAN2_Heartbeat_attributes = {
  .name = "vLED_CAN2_HeartbeatTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
void CAN_Logger_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2){}
static void CAN_Listen(void *argument){}

/* BEGIN vCAN_log */
/**
  * @brief  Log incoming CAN bus traffic.
  * @param  argument: Not used
  * @retval None
  */
void vCAN_log(void *argument)
{
  /* CODE BEGIN */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* CODE END */
}
/* END vCAN_log */


/* BEGIN vLED_HeartbeatOnCanRx */
/**
  * @brief  Blink a LED in heartbeat mode when there is CAN traffic detected.
  * @param  argument: Not used
  * @retval None
  */
void vLED_HeartbeatOnCanRx(void *argument)
{
  /* CODE BEGIN */
  LED_Config *led = (LED_Config*)argument;

  for(;;)
  {
    HAL_GPIO_TogglePin(led->port, led->pin);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  /* CODE END */
}
/* vLED_HeartbeatOnCanRx */
