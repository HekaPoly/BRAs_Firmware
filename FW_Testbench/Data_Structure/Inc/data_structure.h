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

#include "stdbool.h"
#include "stdint.h"
#include <float.h>
#include "cmsis_os.h"


/* Constants */
/**
 * @brief Data structure to contain sensor and control values of each joint
 * 
 */
typedef struct
{
	uint32_t encoder_value;
	uint32_t gyro_value;
	uint32_t motor_speed;
	uint32_t motor_angle_to_reach;
} Data;

/* Function protoytpes */
Data * DataStruct_Get(void);
void DataStruct_ReleaseSemaphore(void);
