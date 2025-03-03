/******************************************************************************
 * @file         bsp_key.c
 * @brief        Key control source file
 * @details      Implements functions to get the state of a key.
 * @author       lqz
 * @date         2025/2/18
 * @version      v1.0
 *****************************************************************************/

 /* Includes ------------------------------------------------------------------*/
#include "bsp_key.h"

/* function prototypes -----------------------------------------------*/ 
static key_state_t key_get (void);
 
/* Defines ------------------------------------------------------------------*/ 
bsp_key_interface_t bsp_key_interface = {
	   .pf_key_get = key_get,
};

/**
 * @brief Get the current state of the key.
 * @param void
 * @return key_state_t The state of the key, either KEY_PRESS or KEY_RELEASE.
 */
static key_state_t key_get (void){
	// Check the GPIO pin state and return corresponding key state
    if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET) {
        return KEY_RELEASE;  // Key is released (assuming the key pulls pin low when pressed)
    } else {
        return KEY_PRESS;    // Key is pressed
    }
}
