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


/* Constants */

typedef struct Data_t
{
	uint32_t encoder_value;
	uint32_t gyro_value;
	uint32_t motor_speed;
	uint32_t motor_angle_to_reach;
} data;

/* Function protoytpes */
struct Data_t * DataStruct_Get(void);
struct Data_t * DataStruct_Release(void);
