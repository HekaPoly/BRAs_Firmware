/**
 * @file motor_control.h
 * @author HEKA
 * @brief Header file for the motor control functions
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include "tim.h"
#include "stdbool.h"
#include <float.h>
#include <stdlib.h>

/* Constants */
#define FREQ_MAX_HZ 14000
#define PSC 9
#define FREQ_CLK_HZ 72000000
#define DEGREES_PER_PULSE_WITHOUT_GEARBOX 0.225f
#define DEGREES_PER_PULSE_WITH_GEARBOX 0.015f

/* Structs and enums */
/**
 * Enum describing the current direction in which a motor is rotating
 */
typedef enum
{
	MOTOR_DIRECTION_CLOCKWISE 			= 0,
	MOTOR_DIRECTION_COUNTERCLOCKWISE 	= 1
} Motor_Direction;

/**
 * Enum describing the current state of the motor
 */
typedef enum
{
	MOTOR_STATE_WAITING_FOR_SEMAPHORE 	= -2,
	MOTOR_STATE_TEMPERATURE_ERROR 		= -1,
	MOTOR_STATE_OK 						= 0
} Motor_State;

/**
 * Structure describing a motor
 */
typedef struct
{
	bool is_motor_initialized;
	TIM_HandleTypeDef * motor_timer_handle;
	Motor_Direction motor_direction;
	float deg_per_turn;
	uint32_t motor_timer_channel;
	uint16_t nb_pulse;
	uint16_t delay;
} Motor;

/* Function declaration */
void MotorControl_Init(void);
Motor_State MotorControl_Task(void);

#endif /* __MOTOR_CONTROL_H__ */


