/**
 * @file gyroscope.h
 * @author HEKA
 * @brief Header file for the gyroscope functions
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
#define NUM_GYRO 1


/* Structs and enums */
/**
 * Enum describing 
 */


/**
 * Enum describing 
 */


/**
 * Structure describing a gyroscope
 */
typedef struct
{
	bool is_gyroscope_initialized;
	TIM_HandleTypeDef * gyroscope_timer_handle;
	uint32_t gyroscope_timer_channel; 
	uint16_t delay;
} Gyroscope;

/* Function declaration */



