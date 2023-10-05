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

#include "main.h"
#include "tim.h"

/* Function implementation */
/**
 * @brief Initialization function for all stepper motors
 * 
 */
void MotorControl_Init(void)
{
    /* Initialize all stepper motors (PWM, Direction GPIO and Enable GPIO) */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	TIM2->ARR = 2500;
	TIM2->CCR1 = TIM2->ARR  / 2;
}

/**
 * @brief Task to control the stepper motors in manual mode
 * 
 */
void MotorControl_Task(void)
{
    
}
