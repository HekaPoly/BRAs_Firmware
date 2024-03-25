/**
 * @file pid.c
 * @author HEKA
 * @brief PID implementation to individually control every motor's speed
 * 
 * @date 2024-02-29
 * 
 * @copyright Copyright HEKA (c) 2024
 * 
 */

#include "pid.h"

/* Static function declaration */
static void Modify_Direction(int16_t difference_deg, Motor * motor);
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired_percent);

/* Function implementation */
/**
 * @brief PID algorithm to modify the speed value of the motor depending on its current position
 * 
 * @param[in] difference_deg    The difference between the current angle of the motor and its desired angle value given by the robotics team 
 * @param[out] current_motor    The current motor being controlled by the PID algorithm
 */
void Pid_Algorithm(int difference_deg, Motor * current_motor)
{
    Modify_Direction(difference_deg, current_motor);
    Modify_Speed(difference_deg, 100);

    
}


/**
 * @brief Modifies the direction signal controlling the turning direction of the motor
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 * @param[out] motor            The current motor being controlled
 *
 */
static void Modify_Direction(int16_t difference_deg, Motor * motor)
{
	if (difference_deg < 0)
	{
		motor->motor_direction = MOTOR_DIRECTION_COUNTERCLOCKWISE;
	}
	else if (difference_deg > 0)
	{
		motor->motor_direction = MOTOR_DIRECTION_CLOCKWISE;
	}
	else
	{
		/* Do nothing here */
	}

	if (motor->motor_direction == MOTOR_DIRECTION_CLOCKWISE)
	{
		HAL_GPIO_WritePin(DIR_Motor_1_GPIO_Port, DIR_Motor_1_Pin, GPIO_PIN_SET);
	}
    else if (motor->motor_direction == MOTOR_DIRECTION_COUNTERCLOCKWISE)
	{
    	HAL_GPIO_WritePin(DIR_Motor_1_GPIO_Port, DIR_Motor_1_Pin, GPIO_PIN_RESET);
	}
    else
    {
        /* Do nothing here */
    }
}


/**
 * @brief Calculation of new ARR value to give the PWM in order to modifiy the motor's speed
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 *
 */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired_percent)
{

}
