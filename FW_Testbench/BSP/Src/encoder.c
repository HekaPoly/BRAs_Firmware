/**
 * @file encoder.c
 * @author HEKA
 * @brief encoder implementation
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "../Inc/encoder.h"
#include "main.h"
#include "../../Data_Structure/Inc/data_structure.h"

/* Constants */
#define FACTOR_SECONDS_TO_MS 1000

/* Private functions declaration */

/* Global variables */
Encoder g_base_encoder =
{
	.is_encoder_initialized = false,
};

/* Function implementation */
/**
 * @brief Initialization function for all encoders
 * 
 */
void Encoder_Init(void)
{
    /* Initialize all encoders (PWM, Direction GPIO and Enable GPIO) */
    g_base_encoder.encoder_timer_handle = &htim3;
	g_base_encoder.encoder_timer_channel = TIM_CHANNEL_1; // doit on mettre un autre channel : TIM3 channel 1 et 2 ?
	g_base_encoder.delay = 0u;
}

/**
 * @brief Task to control the encoder 
 * 
 */

Encoder_State Encoder_Task(void)
{
	Data * data_structure = DataStruct_Get();
	if (data_structure == NULL)
	{
		return ENCODER_STATE_WAITING_FOR_SEMAPHORE;
	}

	/* Test pour valider l'écriture et la lecture de la structure de données */
	data_structure->encoder_value = 3000;

	DataStruct_ReleaseSemaphore();

	return ENCODER_STATE_OK;
}
