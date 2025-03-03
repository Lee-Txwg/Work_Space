/******************************************************************************
 * @file    task_led.h
 * @brief   Header file for the LED task.
 *          This file defines the interface for the LED task.
 * 
 * @note    The LED task controls the state of the LED based on the system state.
 * 
 * @version 1.0
 * @date    2025-02-18
 * @author  Your Name
 ******************************************************************************/

 #ifndef __TASK_LED_H__
 #define __TASK_LED_H__
 
 //******************************** Includes *********************************//
#include "main.h"
#include "task_key.h"
#include "bsp_led.h"

#include "FreeRTOS.h"
#include "queue.h"
//********************************* Includes *********************************//


//******************************** Defines **********************************//
#define LED_QUEUE_SIZE 2

//******************************** Defines **********************************//


//******************************** Declaring ********************************//
typedef struct
{
    void (*pf_led_task_init) (void);
    void (*pf_led_task     ) (void);
    uint32_t *        p_blink_count;
}task_led_interface_t;
extern task_led_interface_t task_led_interface;

extern QueueHandle_t g_led_queue;
//******************************** Declaring ********************************//


 #endif // __TASK_LED_H__
