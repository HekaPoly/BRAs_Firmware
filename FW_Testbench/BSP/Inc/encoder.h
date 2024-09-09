/**
 * @file encoder.h
 * @author HEKA
 * @brief Header file for the encoders management
 *
 * @date 2024-03-28
 * 
 * @copyright Copyright HEKA (c) 2024
 * 
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "tim.h"

#include <stdlib.h>

/* Constants */
#define DEGREES_PER_TURN 360
#define OVERFLOW 2000
#define DEGREES_PER_COUNTER_INCREMENTS (360.0f / (640.0f * 4))

/* Structs and enums */
typedef struct 
{
	TIM_HandleTypeDef * htim;
	float encoder_position_degrees;
	uint32_t encoder_last_counter_value;
	int32_t encoder_velocity;
	int32_t encoder_position_non_converted;
} Encoder;

/* Function declaration */
void Encoder_Init(void);
void Encoder_Task(void);
void Encoder_Update(Encoder * current_encoder);
void Encoder_Reset(Encoder * current_encoder);


#endif /* __ENCODER_H__ */
