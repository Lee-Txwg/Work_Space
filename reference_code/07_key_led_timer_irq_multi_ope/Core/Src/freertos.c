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
#include "task_led.h"
#include "task_key.h"
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
/* Definitions for KeyTask */
osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LedTask */
osThreadId_t LedTaskHandle;
const osThreadAttr_t LedTask_attributes = {
  .name = "LedTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Test_Task */
osThreadId_t Test_TaskHandle;
const osThreadAttr_t Test_Task_attributes = {
  .name = "Test_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for key_queue */
osMessageQueueId_t key_queueHandle;
const osMessageQueueAttr_t key_queue_attributes = {
  .name = "key_queue"
};
/* Definitions for led_queue */
osMessageQueueId_t led_queueHandle;
const osMessageQueueAttr_t led_queue_attributes = {
  .name = "led_queue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void validate_key_task_with_delay(void);
static void led_task_test(void);
/* USER CODE END FunctionPrototypes */

void keytask(void *argument);
void ledtask(void *argument);
void test_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of key_queue */
  key_queueHandle = osMessageQueueNew (4, sizeof(uint16_t), &key_queue_attributes);

  /* creation of led_queue */
  led_queueHandle = osMessageQueueNew (4, sizeof(uint16_t), &led_queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of KeyTask */
  KeyTaskHandle = osThreadNew(keytask, NULL, &KeyTask_attributes);

  /* creation of LedTask */
  LedTaskHandle = osThreadNew(ledtask, NULL, &LedTask_attributes);

  /* creation of Test_Task */
  // Test_TaskHandle = osThreadNew(test_task, NULL, &Test_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_keytask */
/**
  * @brief  Function implementing the KeyTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_keytask */
void keytask(void *argument)
{
  /* USER CODE BEGIN keytask */
  task_key_interface.pf_key_task_init();
  /* Infinite loop */
  for(;;)
  {
    printf("key_task is running at [%d] tick.\r\n",HAL_GetTick());
    task_key_interface.pf_key_task();
  }
  /* USER CODE END keytask */
}

/* USER CODE BEGIN Header_ledtask */
/**
* @brief Function implementing the LedTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ledtask */
void ledtask(void *argument)
{
  /* USER CODE BEGIN ledtask */
  bsp_led_interface.pf_led_init();
  task_led_interface.pf_led_task_init();
  /* Infinite loop */
  for(;;)
  {
    printf("led_task is running at [%d] tick.\r\n",HAL_GetTick());
    task_led_interface.pf_led_task();
  }
  /* USER CODE END ledtask */
}

/* USER CODE BEGIN Header_test_task */
/**
* @brief Function implementing the Test_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_test_task */
void test_task(void *argument)
{
  /* USER CODE BEGIN test_task */
  /* Infinite loop */
  for(;;)
  {
    printf("test_task is running at [%d] tick.\r\n",HAL_GetTick());
    validate_key_task_with_delay();
    // osDelay(1);
  }
  /* USER CODE END test_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// Test function to validate the behavior of key_task
// Test function to validate the behavior of key_task with delays
static void validate_key_task_with_delay(void)
{
    // Simulate key press and release events with various durations

    // Test: Key press with short duration (debouncing)
    key_event_t key_event_1 = { HAL_GetTick(), FALLING_EDGE };
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_1, 0))
    {
        printf("TEST TASK:Send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }

    key_event_t key_event_2 = { HAL_GetTick() + 5, RISING_EDGE }; // 5ms duration
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_2, 0))
    {
        printf("TEST TASK:Send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // Delay 5 seconds before next event

    // Test: Key press with short duration (short press)
    key_event_1.systickcount = HAL_GetTick();
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_1, 0))
    {
        printf("TEST TASK:Send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }

    key_event_2.systickcount = HAL_GetTick() + 1000; // 1000ms duration
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_2, 0))
    {
        printf("TEST TASK:Send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // Delay 5 seconds before next event

    // Test: Key press with long duration (long press)
    key_event_1.systickcount = HAL_GetTick();
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_1, 0))
    {
        printf("TEST TASK:Send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }

    key_event_2.systickcount = HAL_GetTick() + 3000; // 3000ms duration
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_2, 0))
    {
        printf("TEST TASK:Send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // Delay 5 seconds before next event

    // Test: Key press with excessive duration (no action)
    key_event_1.systickcount = HAL_GetTick();
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_1, 0))
    {
        printf("TEST TASK:Send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_1 (FALLING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }

    key_event_2.systickcount = HAL_GetTick() + 6000; // 6000ms duration
    if(pdTRUE == xQueueSend(g_key_queue, &key_event_2, 0))
    {
        printf("TEST TASK:Send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    else
    {
        printf("Failed to send key_event_2 (RISING_EDGE) to queue at [%d] tick.\n", HAL_GetTick());
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // Delay 5 seconds before next event
}


static void led_task_test(void) {
  key_press_type_t key_press_type;
  // 测试不同的按键状�??? 

  //1.短按
  key_press_type = SHORT_PRESS;
/*   taskENTER_CRITICAL(); */
  if(pdTRUE == xQueueSend(g_led_queue,&key_press_type,0))
  {
    printf("Send SHORT_PRESS to queue at [%d] tick.\n",HAL_GetTick());
  }
  else
  {
    printf("Failed to send SHORT_PRESS to queue at [%d] tick.\n",HAL_GetTick());
  }
  /* taskEXIT_CRITICAL(); */
  vTaskDelay(3000);  // Delay 3 seconds

  //2.长按
  key_press_type = LONG_PRESS;
 /*  taskENTER_CRITICAL(); */
  if(pdTRUE == xQueueSend(g_led_queue,&key_press_type,0))
  {
    printf("Send LONG_PRESS to queue at [%d] tick.\n",HAL_GetTick());
  }
  else
  {
    printf("Failed to send LONG_PRESS to queue at [%d] tick.\n",HAL_GetTick());
  }
  /* taskEXIT_CRITICAL(); */
  vTaskDelay(3000);  // Delay 3 seconds

  //3.未按�???
  /* taskENTER_CRITICAL(); */
  key_press_type = NO_PRESS;
  if(pdTRUE == xQueueSend(g_led_queue,&key_press_type,0))
  {
    printf("Send NO_PRESS to queue at [%d] tick.\n",HAL_GetTick());
  }
  else{
    printf("Failed to send NO_PRESS to queue at [%d] tick.\n",HAL_GetTick());
  }
  /* taskEXIT_CRITICAL(); */
  vTaskDelay(3000);  // Delay 3 seconds
}
/* USER CODE END Application */

