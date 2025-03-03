/******************************************************************************
 * @file    bsp_key.h
 * @brief   Header file for the key interface and state management.
 *          This file defines the key state enumeration, the structure 
 *          for key interface, and the function pointer for retrieving 
 *          the key state.
 * 
 * @note    This file is part of the BSP (Board Support Package) for key input 
 *          handling. It provides an abstraction layer to interact with the 
 *          physical key (button) hardware, defining key press and release states.
 * 
 * @version 1.0
 * @date    2025-02-18
 * @author  lqz
 ******************************************************************************/

#ifndef __BSP_KEY_H__  //Avoid repeated including same files later
#define __BSP_KEY_H__

//******************************** Includes *********************************//
#include "main.h"
//******************************** Includes *********************************//


//******************************** Defines **********************************//
// Enum to represent the key state
typedef enum {
  KEY_NULL = 0,      // No key press
  KEY_PRESS,         // Key is pressed
  KEY_RELEASE        // Key is released
} key_state_t;

// Enum representing key press status (for short press, long press, etc.)
typedef enum {
  SHORT_PRESS,
  LONG_PRESS,
  NO_PRESS  // 表示没有按下
} key_press_type_t;
//******************************** Defines **********************************//


//******************************** Declaring ********************************//
typedef struct
{
    key_state_t (*pf_key_get) (void);
}bsp_key_interface_t;

extern bsp_key_interface_t bsp_key_interface;

//******************************** Declaring ********************************//


#endif //End of __BSP_KEY_H__
