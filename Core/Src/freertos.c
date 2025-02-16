/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern EventGroupHandle_t osEventGroup;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LED_TASKHandle;
osThreadId DJI_TASKHandle;
osThreadId IMU_TASKHandle;
osThreadId ERROR_TASKHandle;
osThreadId DETECT_TASKHandle;
osThreadId CHASSIS_TASKHandle;
osThreadId REMOTE_CTRL_TASHandle;
osThreadId LK_TASKHandle;
osThreadId PC_TaskHandle;
osThreadId IMAGEATRANS_TASHandle;
osThreadId JUDGE_TASKHandle;
osThreadId TRANSMIT_TASKHandle;
osThreadId KB_TASKHandle;
osMutexId CAN1MutexHandle;
osMutexId CAN2MutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void OS_LedTask(void const * argument);
void OS_DJITask(void const * argument);
void OS_IMUTask(void const * argument);
void OS_ErrorTask(void const * argument);
void OS_DetectTask(void const * argument);
void OS_ChassisTask(void const * argument);
void OS_RemoteCtrlTask(void const * argument);
void OS_LKTask(void const * argument);
void OS_PCTask(void const * argument);
void OS_ImageTransTask(void const * argument);
void OS_JudgeTask(void const * argument);
void OS_TransmitTask(void const * argument);
void OS_KbTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    osEventGroup = xEventGroupCreate();

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of CAN1Mutex */
  osMutexDef(CAN1Mutex);
  CAN1MutexHandle = osMutexCreate(osMutex(CAN1Mutex));

  /* definition and creation of CAN2Mutex */
  osMutexDef(CAN2Mutex);
  CAN2MutexHandle = osMutexCreate(osMutex(CAN2Mutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityRealtime, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LED_TASK */
  osThreadDef(LED_TASK, OS_LedTask, osPriorityBelowNormal, 0, 256);
  LED_TASKHandle = osThreadCreate(osThread(LED_TASK), NULL);

  /* definition and creation of DJI_TASK */
  osThreadDef(DJI_TASK, OS_DJITask, osPriorityNormal, 0, 512);
  DJI_TASKHandle = osThreadCreate(osThread(DJI_TASK), NULL);

  /* definition and creation of IMU_TASK */
  osThreadDef(IMU_TASK, OS_IMUTask, osPriorityHigh, 0, 528);
  IMU_TASKHandle = osThreadCreate(osThread(IMU_TASK), NULL);

  /* definition and creation of ERROR_TASK */
  osThreadDef(ERROR_TASK, OS_ErrorTask, osPriorityHigh, 0, 256);
  ERROR_TASKHandle = osThreadCreate(osThread(ERROR_TASK), NULL);

  /* definition and creation of DETECT_TASK */
  osThreadDef(DETECT_TASK, OS_DetectTask, osPriorityBelowNormal, 0, 256);
  DETECT_TASKHandle = osThreadCreate(osThread(DETECT_TASK), NULL);

  /* definition and creation of CHASSIS_TASK */
  osThreadDef(CHASSIS_TASK, OS_ChassisTask, osPriorityNormal, 0, 256);
  CHASSIS_TASKHandle = osThreadCreate(osThread(CHASSIS_TASK), NULL);

  /* definition and creation of REMOTE_CTRL_TAS */
  osThreadDef(REMOTE_CTRL_TAS, OS_RemoteCtrlTask, osPriorityBelowNormal, 0, 256);
  REMOTE_CTRL_TASHandle = osThreadCreate(osThread(REMOTE_CTRL_TAS), NULL);

  /* definition and creation of LK_TASK */
  osThreadDef(LK_TASK, OS_LKTask, osPriorityNormal, 0, 384);
  LK_TASKHandle = osThreadCreate(osThread(LK_TASK), NULL);

  /* definition and creation of PC_Task */
  osThreadDef(PC_Task, OS_PCTask, osPriorityNormal, 0, 384);
  PC_TaskHandle = osThreadCreate(osThread(PC_Task), NULL);

  /* definition and creation of IMAGEATRANS_TAS */
  osThreadDef(IMAGEATRANS_TAS, OS_ImageTransTask, osPriorityNormal, 0, 512);
  IMAGEATRANS_TASHandle = osThreadCreate(osThread(IMAGEATRANS_TAS), NULL);

  /* definition and creation of JUDGE_TASK */
  osThreadDef(JUDGE_TASK, OS_JudgeTask, osPriorityNormal, 0, 512);
  JUDGE_TASKHandle = osThreadCreate(osThread(JUDGE_TASK), NULL);

  /* definition and creation of TRANSMIT_TASK */
  osThreadDef(TRANSMIT_TASK, OS_TransmitTask, osPriorityNormal, 0, 256);
  TRANSMIT_TASKHandle = osThreadCreate(osThread(TRANSMIT_TASK), NULL);

  /* definition and creation of KB_TASK */
  osThreadDef(KB_TASK, OS_KbTask, osPriorityNormal, 0, 348);
  KB_TASKHandle = osThreadCreate(osThread(KB_TASK), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
__weak void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_OS_LedTask */
/**
* @brief Function implementing the LED_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_LedTask */
__weak void OS_LedTask(void const * argument)
{
  /* USER CODE BEGIN OS_LedTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_LedTask */
}

/* USER CODE BEGIN Header_OS_DJITask */
/**
* @brief Function implementing the DJI_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_DJITask */
__weak void OS_DJITask(void const * argument)
{
  /* USER CODE BEGIN OS_DJITask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_DJITask */
}

/* USER CODE BEGIN Header_OS_IMUTask */
/**
* @brief Function implementing the IMU_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_IMUTask */
__weak void OS_IMUTask(void const * argument)
{
  /* USER CODE BEGIN OS_IMUTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_IMUTask */
}

/* USER CODE BEGIN Header_OS_ErrorTask */
/**
* @brief Function implementing the ERROR_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_ErrorTask */
__weak void OS_ErrorTask(void const * argument)
{
  /* USER CODE BEGIN OS_ErrorTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_ErrorTask */
}

/* USER CODE BEGIN Header_OS_DetectTask */
/**
* @brief Function implementing the DETECT_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_DetectTask */
__weak void OS_DetectTask(void const * argument)
{
  /* USER CODE BEGIN OS_DetectTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_DetectTask */
}

/* USER CODE BEGIN Header_OS_ChassisTask */
/**
* @brief Function implementing the CHASSIS_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_ChassisTask */
__weak void OS_ChassisTask(void const * argument)
{
  /* USER CODE BEGIN OS_ChassisTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_ChassisTask */
}

/* USER CODE BEGIN Header_OS_RemoteCtrlTask */
/**
* @brief Function implementing the REMOTE_CTRL_TAS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_RemoteCtrlTask */
__weak void OS_RemoteCtrlTask(void const * argument)
{
  /* USER CODE BEGIN OS_RemoteCtrlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_RemoteCtrlTask */
}

/* USER CODE BEGIN Header_OS_LKTask */
/**
* @brief Function implementing the LK_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_LKTask */
__weak void OS_LKTask(void const * argument)
{
  /* USER CODE BEGIN OS_LKTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_LKTask */
}

/* USER CODE BEGIN Header_OS_PCTask */
/**
* @brief Function implementing the PC_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_PCTask */
__weak void OS_PCTask(void const * argument)
{
  /* USER CODE BEGIN OS_PCTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_PCTask */
}

/* USER CODE BEGIN Header_OS_ImageTransTask */
/**
* @brief Function implementing the IMAGEATRANS_TAS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_ImageTransTask */
__weak void OS_ImageTransTask(void const * argument)
{
  /* USER CODE BEGIN OS_ImageTransTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_ImageTransTask */
}

/* USER CODE BEGIN Header_OS_JudgeTask */
/**
* @brief Function implementing the JUDGE_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_JudgeTask */
__weak void OS_JudgeTask(void const * argument)
{
  /* USER CODE BEGIN OS_JudgeTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_JudgeTask */
}

/* USER CODE BEGIN Header_OS_TransmitTask */
/**
* @brief Function implementing the TRANSMIT_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_TransmitTask */
__weak void OS_TransmitTask(void const * argument)
{
  /* USER CODE BEGIN OS_TransmitTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_TransmitTask */
}

/* USER CODE BEGIN Header_OS_KbTask */
/**
* @brief Function implementing the KB_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OS_KbTask */
__weak void OS_KbTask(void const * argument)
{
  /* USER CODE BEGIN OS_KbTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_KbTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
