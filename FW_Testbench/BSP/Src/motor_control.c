/**
 * @file motor_control.c
 * @author HEKA
 * @brief Manual motor control implementation
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "motor_control.h"
#include "main.h"
#include "data_structure.h"
#include "pid.h"

/* Constants */
#define FACTOR_SECONDS_TO_MS 1000

/* Global variables */
Motor g_base_motor =
{
	.is_motor_initialized = false,
};

/* Function implementation */
/**
 * @brief Initialization function for all stepper motors
 * 
 */
void MotorControl_Init(void)
{
    /* Initialize all stepper motors (PWM, Direction GPIO and Enable GPIO) */
	g_base_motor.motor_direction = MOTOR_DIRECTION_CLOCKWISE;
	g_base_motor.motor_timer_handle = &htim2;
	g_base_motor.motor_timer_channel = TIM_CHANNEL_1;

	g_base_motor.deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX;
	g_base_motor.nb_pulse = 0u;
	g_base_motor.delay = 0u;
}

/**
 * @brief Task to control the stepper motors in manual mode
 * 
 */
Motor_State MotorControl_Task(void)
{
	Data * data_structure = DataStruct_Get();
	if (data_structure == NULL)
	{
		return MOTOR_STATE_WAITING_FOR_SEMAPHORE;
	}

	/* Get all desired angle values and maximal speeds from the data structure */

	/* Release data structure semaphore */

	/* For loop for each motor: 
		1- Read the current encoder value for the motor
		2- Call the PID function with the angle difference for the current motor
	*/

	DataStruct_ReleaseSemaphore();

	return MOTOR_STATE_OK;
}

