/**
 * @file data_structure.h
 * @author HEKA
 * @brief Header file for the data structure
 *
 * @date 2023-11-10
 *
 * @copyright Copyright HEKA (c) 2023
 *
 */

#include <float.h>
#include "stdbool.h"
#include "stdint.h"
#include "cmsis_os.h"

#define NUMBER_MOTOR 6

/* Constants */
/**
 * @brief Data structure for any motor at a joint
 */
typedef struct
{
	uint32_t motor_desired_speed_percent;
	uint32_t motor_angle_to_reach_deg;
	uint32_t motor_current_angle_deg;
	uint32_t encoder_value_deg;
} Data_Motor;

/**
 * @brief Data structure for any encoder at a joint
 */
typedef struct
{
	float encoder_degrees;
} Data_Encoder;

/**
 * @brief Data structure to contain sensor and control values of each joint
 * 
 */
typedef struct
{
	bool is_dataset_initialized;
	Data_Encoder encoder_base;
	Data_Motor Data_Motors[NUMBER_MOTOR]; //Name subject to change
	uint32_t gyro_value;
} Data;

/* Function protoytpes */
Data * DataStruct_Get(void);
void DataStruct_ReleaseSemaphore(void);
