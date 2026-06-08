#include "canlog.h"
#include "cmsis_os.h"
#include <string.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/* Definitions for canBus1Listen */
osThreadId_t canBus1ListenHandle;
const osThreadAttr_t canBus1Listen_attributes = {
  .name = "canBus1Listen",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime1,
};
/* Definitions for canBus2Listen */
osThreadId_t canBus2ListenHandle;
const osThreadAttr_t canBus2Listen_attributes = {
  .name = "canBus2Listen",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* CODE END */
}
/* vLED_HeartbeatOnCanRx */
