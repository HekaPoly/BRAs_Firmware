/**
 * @file data_structure.c
 * @author HEKA
 * @brief data structure for encoders, gyroscope sensors,speed and angle to reach
 *
 * @date 2023-11-10
 *
 * @copyright Copyright HEKA (c) 2023
 *
 */

#include "../Inc/data_structure.h"

struct Data_t * data_struct = (struct Data_t*)0x20040000;

/* Function implementation */
/**
 * @ function to return data address
 *
 */
struct Data_t * DataStruct_Get(void)
{
    return data_struct;
}

