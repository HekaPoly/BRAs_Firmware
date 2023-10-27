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

#include "../Inc/motor_control.h"
#include "main.h"


/* Private functions declaration */
static void Modify_Speed(void);
static void Modify_Direction(int16_t difference);

/* Global variables */
struct Motor_t g_base_motor =
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
	g_base_motor.motor_direction = DIRECTION_CLOCKWISE;
	g_base_motor.motor_timer_handle = &htim2;
	g_base_motor.motor_timer_channel = TIM_CHANNEL_1;

	g_base_motor.motor_timer_handle->Instance->ARR = 2500;
	g_base_motor.motor_timer_handle->Instance->CCR1 = g_base_motor.motor_timer_handle->Instance->ARR / 2;

	g_base_motor.motor_angle_to_reach = 0;

	g_base_motor.deg_per_turn = 0.06f;
	g_base_motor.current_motor_angle = 0;
	g_base_motor.nb_pulse = 0;
	g_base_motor.delay = 0;

}

/**
 * @brief Task to control the stepper motors in manual mode
 * 
 */
void MotorControl_Task(void)
{
	int16_t difference = g_base_motor.motor_angle_to_reach - g_base_motor.current_motor_angle;

	if (difference != 0)
	{
		Modify_Direction(difference);
		Modify_Speed();
	}
}

void Modify_Speed(void)
{
	uint16_t new_freq = (g_base_motor.motor_speed_percent * FREQ_MAX_HZ) / 100;
	uint16_t new_arr = (FREQ_CLK_HZ / ((PSC + 1)* new_freq)) - 1;

	g_base_motor.motor_timer_handle->Instance->ARR = new_arr;
	g_base_motor.motor_timer_handle->Instance->CCR1 = g_base_motor.motor_timer_handle->Instance->ARR / 2;

	g_base_motor.nb_pulse = (abs(g_base_motor.motor_angle_to_reach - g_base_motor.current_motor_angle)) / g_base_motor.deg_per_turn ;
	g_base_motor.delay = ((float)g_base_motor.nb_pulse / (float)new_freq) * 1000;

	HAL_TIM_PWM_Start(g_base_motor.motor_timer_handle, g_base_motor.motor_timer_channel);
	HAL_Delay(g_base_motor.delay);
	HAL_TIM_PWM_Stop(g_base_motor.motor_timer_handle, g_base_motor.motor_timer_channel);

	g_base_motor.current_motor_angle = g_base_motor.motor_angle_to_reach;
}

void Modify_Direction(int16_t difference)
{
	if (difference < 0)
	{
		g_base_motor.motor_direction = DIRECTION_COUNTERCLOCKWISE;
	}

	else if (difference > 0)
	{
		g_base_motor.motor_direction = DIRECTION_CLOCKWISE;
	}
	else
	{
		/* nothing do */
	}


	if (g_base_motor.motor_direction == DIRECTION_CLOCKWISE)
	{
		HAL_GPIO_WritePin(DIR_Motor_1_GPIO_Port, DIR_Motor_1_Pin, GPIO_PIN_SET);
	}
    else if (g_base_motor.motor_direction == DIRECTION_COUNTERCLOCKWISE)
	{
    	HAL_GPIO_WritePin(DIR_Motor_1_GPIO_Port, DIR_Motor_1_Pin, GPIO_PIN_RESET);
	}
}
