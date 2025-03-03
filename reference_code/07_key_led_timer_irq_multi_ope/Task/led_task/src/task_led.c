/******************************************************************************
 * @file    task_led.c
 * @brief   Source file for the LED task.
 *          This file implements the LED task which toggles the LED at regular intervals.
 *
 * @note    This task controls the state of the LED and performs toggling periodically.
 *
 * @version 1.0
 * @date    2025-02-18
 * @author  Your Name
 ******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "task_led.h"

/* variables ------------------------------------------------------------------*/
QueueHandle_t g_led_queue;
static uint32_t g_blink_count = 0;

/* function prototypes -----------------------------------------------*/
static void led_task(void);
static void led_task_init(void);
/* Defines ------------------------------------------------------------------*/
task_led_interface_t task_led_interface = {
	.pf_led_task_init = led_task_init,
	.pf_led_task 	  = led_task     ,
	.p_blink_count	  = &g_blink_count
};

/**
 * @brief LED task function that handles key press events to control LED behavior.
 *
 * This function waits for key press events from a queue and performs actions
 * based on the type of key press received. The possible key press types are:
 * - SHORT_PRESS: Toggles the LED state once, causing it to blink.
 * - LONG_PRESS: Toggles the LED state three times, causing it to blink three times.
 * - NO_PRESS: No action is taken.
 *
 * If no data is received from the queue, an error message is printed.
 *
 * @note This function is intended to be run as a FreeRTOS task.
 */
static void led_task(void)
{
	key_press_type_t key_press_type = NO_PRESS;
	// 1.从 led_queue 中接收一个数据项，这个数据项包含一个枚举值，
	//   表示按键的状态（短按、长按等）
	// 1.1如果接收到数据，则根据数据项的内容，调用 bsp_led_interface 中相应的函数来控制 LED 的行为：
	if (pdPASS == xQueueReceive(g_led_queue, &key_press_type, portMAX_DELAY))
	{
		printf("LED TASK:Received data from g_led_queue: [%d] at : %u tick\n", key_press_type, HAL_GetTick());
		switch (key_press_type)
		{
		case SHORT_PRESS: // 1.1.1短按：LED 闪烁一次。
			*task_led_interface.p_blink_count = 1;
			printf("blink_count:[%d]\n", *task_led_interface.p_blink_count);
			// bsp_led_interface.pf_led_toggle();
			// vTaskDelay(500);
			// bsp_led_interface.pf_led_toggle();
			break;

		case LONG_PRESS: // 1.1.2长按：LED 闪烁十次。
			*task_led_interface.p_blink_count = 10;
			printf("blink_count:[%d]\n", *task_led_interface.p_blink_count);
			// for (int i = 0; i < 6; i++)
			// {
			// 	bsp_led_interface.pf_led_toggle();
			// 	vTaskDelay(500);
			// }
			break;

		case NO_PRESS: // 1.1.3未按下去：LED无操作
			// No action
			break;

		default:
			// No action
			break;
		}
	}
	else // 1.2如果没有接收到数据，则什么都不做。
	{
		printf("Error: Failed to receive key press event from the queue.\n");
	}
}

static void led_task_init(void)
{
	g_led_queue = xQueueCreate(LED_QUEUE_SIZE, sizeof(uint32_t));
	if (NULL == g_led_queue)
	{
		printf("Error: Failed to create LED queue.\n");
	}
	else
	{
		printf("LED queue created successfully.\n");
	}
}
