#include "canlog.h"
#include "cmsis_os.h"
#include <string.h>

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

/* BEGIN Header_CAN_Listen */
/**
  * @brief  Function implementing the canBus0Listen thread.
  * @param  argument: Not used
  * @retval None
  */
/* END Header_CAN_Listen */
void CAN_Listen(void *argument)
{
  /* CODE BEGIN */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* CODE END */
}
