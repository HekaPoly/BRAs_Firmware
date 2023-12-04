/**
 * @file gyroscope.c
 * @author HEKA
 * @brief gyroscope implementation
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
Gyroscope g_base_gyroscope =
{
	.is_gyroscope_initialized = false,
};

/* Function implementation */
/**
 * @brief Initialization function for gyroscopes
 * 
 */
void Encoder_Init(void)
{
    /* Initialize all stepper motors (PWM, Direction GPIO and Enable GPIO) */
    g_base_gyroscope.gyroscope_timer_handle = &htim3;
	g_base_gyroscope.gyroscope_timer_channel = TIM_CHANNEL_1; // doit on mettre un autre channel ?
	g_base_gyrosocpe.delay = 0u;
}

/**
 * @brief Task to control the gyroscope
 * 
 */


