/**
 * @file encoder.h
 * @author HEKA
 * @brief Header file for the encoders functions
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "tim.h"
#include "stdbool.h"
#include <float.h>
#include <stdlib.h>

/* Constants */
#define NUM_ENCODERS 3


/* Structs and enums */
/**
 * Enum describing 


/**
 * Enum describing 
 */


/**
 * Structure describing an encoder
 */
typedef struct
{
	bool is_encoder_initialized;
	TIM_HandleTypeDef * encoder_timer_handle;
	uint32_t encoder_timer_channel; 
	uint16_t delay;
} Encoder;

/* Function declaration */



