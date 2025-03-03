/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_key.c
 *
 * @par dependencies
 * - bsp_key.h
 * - stdio.h
 * - stdint.h
 *
 * @author lqz
 *
 * @brief Provide the HAL APIs of KEY and corresponding opetions.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-12-08
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/

/* Includes -----------------------------------------------------------*/
#include "task_key.h"

/* Defines  -----------------------------------------------------------*/

/* function prototypes -----------------------------------------------*/
static void key_task_init (void);    //Function prototype declarations must be preceded
static void key_task	  (void);	 //by statements that initialize the interface.

/* Variables ----------------------------------------------------------*/
QueueHandle_t g_key_queue;
task_key_interface_t task_key_interface = {
	.pf_key_task_init = key_task_init,
	.pf_key_task      = key_task,
};

/* Functions ----------------------------------------------------------*/
task_key_interface_t task_key_interface;

static void key_task_init(void)
{
	// Create a queue capable of containing 3 sizeof(key_event_t) values.
	g_key_queue = xQueueCreate(KEY_QUEUE_SIZE, sizeof(key_event_t));
	if (NULL == g_key_queue)
	{
		printf("Error: Failed to create KEY queue.\n");
	}
	else
	{
		printf("KEY queue created successfully.\n");
	}
}

// APP2: Key_Task
static void key_task(void)
{
	key_event_t 	 key_event     			   ;
	key_press_type_t key_press_type  = NO_PRESS;
	uint32_t         press_duration  = 0       ;
	static uint32_t  last_tick_count 	 = 0       ;

	// (输入）1.无限阻塞接收来自中断发送到key_queue的消息（结构体）
	if(pdPASS != xQueueReceive(g_key_queue, &key_event, portMAX_DELAY)){
		printf("Error: Failed to receive from key queue.\n");
        return;
	}else
	{
		printf("KEY TASK:Received data from g_key_queue at : %u tick\n", HAL_GetTick());
	}
	//(处理）1.1触发方式为falling（此时必定为按下） --> 记录trigger_tick
	if(FALLING_EDGE == key_event.triggermode){
		last_tick_count = key_event.systickcount;
		printf("Falling edge trigger, record trigger time: at [%d] tick.\n",HAL_GetTick());
		return;
	}
	//(处理）1.2触发方式为rising （此时必定为松开） --> 计算key_event.systickcout-trigger_tick
	if(RISING_EDGE == key_event.triggermode){
		press_duration = key_event.systickcount - last_tick_count;
		//1.2.1 <10ms      ,抖动     -> no action
		if(press_duration < 10){
			// no action.
			printf("Key jitter: at [%d] tick.\n",HAL_GetTick());
			return;
		}
		//1.2.2 10~2000ms  ,短按     -> 发送短按消息到led_queue（输出）
		else if(press_duration < 2000)
		{
			key_press_type = SHORT_PRESS;
			if(pdPASS != xQueueSend(g_led_queue, &key_press_type, 0))
			{
				printf("Failed to send SHORT_PRESS to queue at [%d] tick.\n",HAL_GetTick());
			}
			else
			{
				printf("KEY TASK:Send SHORT_PRESS to queue at [%d] tick.\n",HAL_GetTick());
			}
		}
		//1.2.3 2000~5000ms,长按     -> 发送长按消息到led_queue (输出）
		else if(press_duration < 5000)
		{
			key_press_type = LONG_PRESS;
			if(pdPASS != xQueueSend(g_led_queue, &key_press_type, 0))
			{
				printf("Failed to send LONG_PRESS to queue at [%d] tick.\n",HAL_GetTick());
			}
			else
			{
				printf("KEY TASK:Send LONG_PRESS to queue at [%d] tick.\n",HAL_GetTick());
			}
		}
		//1.2.4 >5000ms    ,异常情况  -> no action
		else
		{
			// no action.
			printf("Exception: at [%d] tick.\n",HAL_GetTick());
			return;
		}
	}
}
