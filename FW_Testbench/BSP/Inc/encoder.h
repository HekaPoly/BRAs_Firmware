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

/* Constants */
#define DEGREES_PER_TURN 360

/* Structs and enums */
typedef struct 
{
	TIM_HandleTypeDef * htim;
	uint32_t encoder_last_counter_value;
	int32_t encoder_position_non_converted;
	int32_t encoder_position_degrees;
	int32_t encoder_velocity;
} Encoder;

/* Function declaration */
void Encoder_Init(void);
void Encoder_Task(void);
void Encoder_Update(Encoder * current_encoder);
void Encoder_Reset(Encoder * current_encoder);

#endif /* __ENCODER_H__ */
