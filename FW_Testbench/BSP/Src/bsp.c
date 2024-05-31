/**
 * @file bsp.c
 * @author HEKA
 * @brief Board support package for all hardware-level modules
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "../Inc/bsp.h"
#include "../Inc/motor_control.h"

/**
 * @brief Initialization of the board support package
 * 
 */
void BSP_Init(void)
{
    MotorControl_Init();
}

/**
 * @brief Board support package task repartition
 * 
 */
void BSP_Task(void)
{
	MotorControl_Task();
}
