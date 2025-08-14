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
#include "data_structure.h"

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
	/*
	 * Encoder Pulses Per Revolution (PPR) is set to 640 (on the hardware of the encoder)
	 * It's Count Per Revolution (CPR) is therefore 640 * 4
	 * We set the ARR of the encoder timer to 640 * 4 since it is going to receive a maximum of 640 * 4 counts per 360 degrees
	 */

	uint32_t temp_counter = (uint32_t)((current_encoder->htim->Instance->CNT));
	uint32_t absolute_difference = abs(temp_counter - current_encoder->encoder_last_counter_value);

	if (temp_counter == current_encoder->encoder_last_counter_value)
	{
		current_encoder->encoder_velocity = 0;
	}
	else if (temp_counter > current_encoder->encoder_last_counter_value)
	{
		/* Overflow state */
		if (absolute_difference >= OVERFLOW)
		{
			current_encoder->encoder_velocity = -(current_encoder->encoder_last_counter_value) - (__HAL_TIM_GET_AUTORELOAD(current_encoder->htim) - temp_counter);
		}
		/* Regular upcounting */
		else
		{
			current_encoder->encoder_velocity = temp_counter - current_encoder->encoder_last_counter_value;
		}
	}
	else
	{
		/* Regular downcounting */
		if (absolute_difference < OVERFLOW)
		{
			current_encoder->encoder_velocity = temp_counter - current_encoder->encoder_last_counter_value;
		}
		/* Overflow state */
		else
		{
			current_encoder->encoder_velocity = temp_counter + (__HAL_TIM_GET_AUTORELOAD(current_encoder->htim) - current_encoder->encoder_last_counter_value);
		}
	}

	current_encoder->encoder_position_non_converted = current_encoder->encoder_position_non_converted + current_encoder->encoder_velocity;
	current_encoder->encoder_position_degrees = (current_encoder->encoder_position_non_converted * DEGREES_PER_COUNTER_INCREMENTS);

	current_encoder->encoder_last_counter_value = temp_counter;

	Data * data_structure = DataStruct_Get();
	if (data_structure == NULL)
	{
		//return MOTOR_STATE_WAITING_FOR_SEMAPHORE;
	}
	else
	{
		data_structure->encoder_base.encoder_degrees = current_encoder->encoder_position_degrees;
		DataStruct_ReleaseSemaphore();
	}

}

void Encoder_Reset(Encoder * current_encoder)
{
	current_encoder->encoder_position_non_converted 	= 0;
	current_encoder->encoder_position_degrees 			= 0;
	current_encoder->encoder_velocity 					= 0;
	current_encoder->encoder_last_counter_value 		= 0;
}
