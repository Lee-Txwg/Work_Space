/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_key.h
 * 
 * @par dependencies 
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

#ifndef __TASK_KEY_H__  //Avoid repeated including same files later
#define __TASK_KEY_H__

//******************************** Includes *********************************//
#include "task_led.h"
#include "bsp_key.h" 

#include "FreeRTOS.h"
#include "queue.h"
//******************************** Includes *********************************//


//******************************** Defines **********************************//
#define KEY_QUEUE_SIZE 2

// Define an enumeration type for Key interrupt event trigger mode
typedef enum { 
  FALLING_EDGE,  // Key is pressed 
  RISING_EDGE    // Key is released 
} key_exti_trigger_mode_t; 

// Define a structure for key events 
typedef struct { 
  uint32_t                  systickcount;  // Current system tick time 
  key_exti_trigger_mode_t   triggermode;  // Trigger mode (pressed/released) 
} key_event_t; 
//******************************** Defines **********************************//


//******************************** Declaring ********************************//
typedef struct{
  void (*pf_key_task_init) (void);
  void (*pf_key_task     ) (void);
}task_key_interface_t;
extern task_key_interface_t task_key_interface;

extern QueueHandle_t g_key_queue;
//******************************** Declaring ********************************//

#endif //End of __TASK_KEY_H__
