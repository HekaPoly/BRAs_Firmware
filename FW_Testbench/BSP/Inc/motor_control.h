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

#include "tim.h"
#include "stdbool.h"
#include <float.h>

/* Constants */
#define FREQ_MAX_HZ 3598
#define PSC 9
#define FREQ_CLK_HZ 72000000

/* Structs and enums */
/**
 * Enum describing the current direction in which a motor is rotating
 */
typedef enum
{
	DIRECTION_CLOCKWISE,
	DIRECTION_COUNTERCLOCKWISE
} Motor_Direction_t;

/**
 * Structure describing a motor
 */
typedef struct Motor_t
{
	bool is_motor_initialized;
	TIM_HandleTypeDef * motor_timer_handle;
	uint32_t motor_timer_channel;
	Motor_Direction_t motor_direction;
	uint16_t motor_speed_percent;
	uint16_t motor_angle_to_reach;
	float deg_per_turn;
	uint16_t current_motor_angle;
	uint16_t nb_pulse;
	uint16_t delay;
} Motor;

/* Function implementation */
void MotorControl_Init(void);


void MotorControl_Task(void);


