/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "motor_control.h"
#include "uart.h"
#include "encoder.h"

#include "stdio.h"
#include "usart.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
osThreadId Sensors_TaskHandle;
osThreadId MotorControl_TaHandle;
osThreadId UART_taskHandle;
osSemaphoreId binarySemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartSensorsTask(void const * argument);
void StartMotorControlTask(void const * argument);
void StartUART_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

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

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of binarySemaphore */
  osSemaphoreDef(binarySemaphore);
  binarySemaphoreHandle = osSemaphoreCreate(osSemaphore(binarySemaphore), 1);


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

  /* definition and creation of Sensors_Task */
  osThreadDef(Sensors_Task, StartSensorsTask, osPriorityBelowNormal, 0, 128);
  Sensors_TaskHandle = osThreadCreate(osThread(Sensors_Task), NULL);

  /* definition and creation of MotorControl_Ta */
  osThreadDef(MotorControl_Ta, StartMotorControlTask, osPriorityNormal, 0, 128);
  MotorControl_TaHandle = osThreadCreate(osThread(MotorControl_Ta), NULL);

  /* definition and creation of UART_task */
  osThreadDef(UART_task, StartUART_task, osPriorityAboveNormal, 0, 128);
  UART_taskHandle = osThreadCreate(osThread(UART_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartSensorsTask */
/**
  * @brief  Function implementing the Sensors_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartSensorsTask */
void StartSensorsTask(void const * argument)
{
  /* USER CODE BEGIN StartSensorsTask */
  /* Infinite loop */
	Encoder_Init();
  for(;;)
  {
	  Encoder_Task();
    osDelay(100);
  }
  /* USER CODE END StartSensorsTask */
}

/* USER CODE BEGIN Header_StartMotorControlTask */
/**
* @brief Function implementing the MotorControl_Ta thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorControlTask */
void StartMotorControlTask(void const * argument)
{
  /* USER CODE BEGIN StartMotorControlTask */
  /* Encoder_Init() à rajouter */
  MotorControl_Init();

  /* Infinite loop */
  for(;;)
  {
	/* Encoder_Task() à rajouter */
	MotorControl_Task();
    osDelay(50);
  }
  /* USER CODE END StartMotorControlTask */
}

/* USER CODE BEGIN Header_StartUART_task */
/**
* @brief Function implementing the UART_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUART_task */
void StartUART_task(void const * argument)
{
  /* USER CODE BEGIN StartUART_task */
  UART_Init();

  /* Infinite loop */
  for(;;)
  {
	UART_Task();
    osDelay(75);
  }
  /* USER CODE END StartUART_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
