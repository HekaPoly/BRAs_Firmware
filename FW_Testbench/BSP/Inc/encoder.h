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
// Enum describing the encoder state 
 
typedef enum
{
	ENCODER_STATE_WAITING_FOR_SEMAPHORE 	= -2,
	ENCODER_STATE_OK 						= 0
} Encoder_State;


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

void Encoder_Init(void);
Encoder_State Encoder_Task(void);
