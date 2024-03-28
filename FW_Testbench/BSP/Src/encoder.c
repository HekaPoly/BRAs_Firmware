/**
 * @file encoder.c
 * @author HEKA
 * @brief Manual motor control implementation
 *
 * @date 2024-03-28
 *
 * @copyright Copyright HEKA (c) 2024
 *
 */

#include "encoder.h"

/* Global variables */
Encoder g_base_motor_encoder =
{
};

/* Function implementation */
void Encoder_Init(void)
{
	g_base_motor_encoder.htim = &htim3;
	Encoder_Reset(&g_base_motor_encoder);
}

void Encoder_Task(void)
{
	Encoder_Update(&g_base_motor_encoder);
}

void Encoder_Update(Encoder * current_encoder)
{
	uint32_t temp_counter = (uint32_t)((current_encoder->htim->Instance->CNT) >> 2);

	/* Verify overflow possiblities */
	if (temp_counter > current_encoder->encoder_last_counter_value)
	{
		/* Overflow state */
		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(current_encoder->htim))
		{
			current_encoder->encoder_velocity = -(current_encoder->encoder_last_counter_value) - (__HAL_TIM_GET_AUTORELOAD(current_encoder->htim) - temp_counter);
		}
		else
		{
			current_encoder->encoder_velocity = temp_counter - current_encoder->encoder_last_counter_value;
		}
	}
	else if (temp_counter < current_encoder->encoder_last_counter_value)
	{
		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(current_encoder->htim))
		{
			current_encoder->encoder_velocity = temp_counter - current_encoder->encoder_last_counter_value;
		}
		/* Overflow state */
		else
		{
			current_encoder->encoder_velocity = temp_counter + (__HAL_TIM_GET_AUTORELOAD(current_encoder->htim) - current_encoder->encoder_last_counter_value);
		}
	}

	current_encoder->encoder_position_non_converted = current_encoder->encoder_last_counter_value + current_encoder->encoder_velocity;
	current_encoder->encoder_position_degrees = (( current_encoder->encoder_position_non_converted * DEGREES_PER_TURN) / 1280) % DEGREES_PER_TURN;
}

void Encoder_Reset(Encoder * current_encoder)
{
	current_encoder->encoder_position_non_converted 	= 0;
	current_encoder->encoder_position_degrees 			= 0;
	current_encoder->encoder_velocity 					= 0;
	current_encoder->encoder_last_counter_value 		= 0;
}
