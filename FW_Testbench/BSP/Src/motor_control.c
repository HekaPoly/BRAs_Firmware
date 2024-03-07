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

/* Private functions declaration */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired);
static void Modify_Direction(int16_t difference_deg);

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

	/*
	int16_t difference_deg = data_structure->motor_base.motor_angle_to_reach_deg - data_structure->motor_base.motor_current_angle_deg;

	if (difference_deg != 0)
	{
		Modify_Direction(difference_deg);
		Modify_Speed(difference_deg, data_structure->motor_base.motor_desired_speed_percent);

		data_structure->motor_base.motor_current_angle_deg = data_structure->motor_base.motor_angle_to_reach_deg;
	}
	*/

	DataStruct_ReleaseSemaphore();

	return MOTOR_STATE_OK;
}

/**
 * @brief Calculation of new ARR value to give the PWM in order to modifiy the motor's speed
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 *
 */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired_percent)
{
	uint16_t new_freq = (motor_speed_desired_percent * FREQ_MAX_HZ) / 100;
	uint16_t new_arr = (FREQ_CLK_HZ / ((PSC + 1)* new_freq)) - 1;

	g_base_motor.motor_timer_handle->Instance->ARR = new_arr;
	g_base_motor.motor_timer_handle->Instance->CCR1 = g_base_motor.motor_timer_handle->Instance->ARR / 2;

	g_base_motor.nb_pulse = abs(difference_deg) / g_base_motor.deg_per_turn;
	g_base_motor.delay = ((float)g_base_motor.nb_pulse / (float)new_freq) * FACTOR_SECONDS_TO_MS;

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	HAL_TIM_PWM_Start(g_base_motor.motor_timer_handle, g_base_motor.motor_timer_channel);
	HAL_Delay(g_base_motor.delay);
	HAL_TIM_PWM_Stop(g_base_motor.motor_timer_handle, g_base_motor.motor_timer_channel);

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

