/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UML_FSM.hpp"
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Definitions for TaskFSM1 */
osThreadId_t TaskFSM1Handle;
uint32_t TaskFSM1Buffer[ 128 ];
osStaticThreadDef_t TaskFSM1ControlBlock;
const osThreadAttr_t TaskFSM1_attributes = {
  .name = "TaskFSM1",
  .cb_mem = &TaskFSM1ControlBlock,
  .cb_size = sizeof(TaskFSM1ControlBlock),
  .stack_mem = &TaskFSM1Buffer[0],
  .stack_size = sizeof(TaskFSM1Buffer),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for TaskFSM2 */
osThreadId_t TaskFSM2Handle;
uint32_t TaskFSM2Buffer[ 128 ];
osStaticThreadDef_t TaskFSM2ControlBlock;
const osThreadAttr_t TaskFSM2_attributes = {
  .name = "TaskFSM2",
  .cb_mem = &TaskFSM2ControlBlock,
  .cb_size = sizeof(TaskFSM2ControlBlock),
  .stack_mem = &TaskFSM2Buffer[0],
  .stack_size = sizeof(TaskFSM2Buffer),
  .priority = (osPriority_t) osPriorityBelowNormal2,
};
/* Definitions for myTimer */
osTimerId_t myTimerHandle;
osStaticTimerDef_t myTimerControlBlock;
const osTimerAttr_t myTimer_attributes = {
  .name = "myTimer",
  .cb_mem = &myTimerControlBlock,
  .cb_size = sizeof(myTimerControlBlock),
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void fTaskFSM1(void *argument);
void fTaskFSM2(void *argument);
void TimerCallback(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum
{
	STATE_FSM1,
	STATE_FSM2,
STATE_BLINK_SHORT,
STATE_BLINK_LONG,
STATE_BLINK_REAL,
STATE_WHAIT_TIMER,
STATE_NUM
}State_t;

typedef enum
{
EVENT_ERROR,
EVENT_TYMER_UPDATE,
EVENT_NUM
}Event_t;

typedef enum
{
MACHINE_1,
MACHINE_2,
MACHINE_NUM
}Machine_t;








void f_stateBlinkShort();

void f_stateBlinkLong();

void f_stateBlinkReal();


void f_stateWhaitTimer();



FiniteStateMachine fsm1;
StandardStates stateBlinkShort= fsm1.createStateStandart(STATE_BLINK_SHORT, STATE_BLINK_LONG, f_stateBlinkShort);
StandardStates stateBlinkLong = fsm1.createStateStandart(STATE_BLINK_LONG, STATE_BLINK_SHORT, f_stateBlinkLong);

FiniteStateMachine fsm2;

StandardStates stateBlinkReal= fsm2.createStateStandart(STATE_BLINK_REAL, STATE_WHAIT_TIMER, f_stateBlinkReal);
EventStates stateWhaitTimer = fsm2.createStateEvent(STATE_WHAIT_TIMER, f_stateWhaitTimer);


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	fsm1.setStartState(STATE_BLINK_SHORT);
	fsm2.setStartState(STATE_WHAIT_TIMER);
	stateWhaitTimer.addEvent(EVENT_TYMER_UPDATE, STATE_BLINK_REAL);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer */
  myTimerHandle = osTimerNew(TimerCallback, osTimerPeriodic, NULL, &myTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TaskFSM1 */
  TaskFSM1Handle = osThreadNew(fTaskFSM1, NULL, &TaskFSM1_attributes);

  /* creation of TaskFSM2 */
  TaskFSM2Handle = osThreadNew(fTaskFSM2, NULL, &TaskFSM2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void f_stateBlinkShort()
{
	const uint32_t xFrequency = 500; // 500 миллисекунд
	const TickType_t xTransitionTime = pdMS_TO_TICKS(6000); // 6 секунд
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while((xTaskGetTickCount() - xLastWakeTime) < xTransitionTime)
	{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Выполнение действий для состояния 1
	osDelay(xFrequency); // Задержка в тиках времени FreeRTOS
	}
	fsm1.next();
	//fsm1.pState->end();
}

void f_stateBlinkLong()
{
	const uint32_t xFrequency = 1000; // 500 миллисекунд
	const TickType_t xTransitionTime = pdMS_TO_TICKS(6000); // 6 секунд
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while((xTaskGetTickCount() - xLastWakeTime) < xTransitionTime)
	{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Выполнение действий для состояния 1
	osDelay(xFrequency); // Задержка в тиках времени FreeRTOS
	}
	fsm1.next();
}

void f_stateBlinkReal()
{
	const uint32_t xFrequency = 100; // 100 миллисекунд
	const TickType_t xTransitionTime = pdMS_TO_TICKS(2000); // 2 секунд
	TickType_t xLastWakeTime = xTaskGetTickCount();
	  while((xTaskGetTickCount() - xLastWakeTime) < xTransitionTime)
	  {
		  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Выполнение действий для состояния 1
		  osDelay(xFrequency); // Задержка в тиках времени FreeRTOS
	  }
	 fsm2.next();

}

void f_stateWhaitTimer()
{
	osTimerStart(myTimerHandle, pdMS_TO_TICKS(10000));


	fsm2.next();

}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_fTaskFSM1 */
/**
  * @brief  Function implementing the TaskFSM1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_fTaskFSM1 */
void fTaskFSM1(void *argument)
{
  /* USER CODE BEGIN 5 */
fsm1.stateMachine();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_fTaskFSM2 */
/**
* @brief Function implementing the TaskFSM2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fTaskFSM2 */
void fTaskFSM2(void *argument)
{
  /* USER CODE BEGIN fTaskFSM2 */
fsm2.stateMachine();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END fTaskFSM2 */
}

/* TimerCallback function */
void TimerCallback(void *argument)
{
  /* USER CODE BEGIN TimerCallback */
fsm2.sendEvent(EVENT_TYMER_UPDATE);
  /* USER CODE END TimerCallback */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
