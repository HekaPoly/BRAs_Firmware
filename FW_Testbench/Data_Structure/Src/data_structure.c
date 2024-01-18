/**
 * @file data_structure.c
 * @author HEKA
 * @brief Data structure for all sensor values and event information variables
 *
 * @date 2023-11-10
 *
 * @copyright Copyright HEKA (c) 2023
 *
 */

#include "string.h"
#include "data_structure.h"
#include "cmsis_os.h"

/* External variables */
extern osSemaphoreId binarySemaphoreHandle;

/* Data structure address declaration in .c to keep other files from reaching it */
Data g_data_struct = {0};

/* Function implementation */
/**
 * @brief Returns data structure address if the semaphore is not taken
 * 
 * @return The data structure address when the semaphore is available, else returns a null pointer
 */
Data * DataStruct_Get(void)
{
	if (g_data_struct.is_dataset_initialized != true)
	{
		memset(&g_data_struct, 0, sizeof(Data));
		g_data_struct.is_dataset_initialized = true;
	}

	if (osSemaphoreWait(binarySemaphoreHandle, 0) == osOK)
	{
	    return &g_data_struct;
	}
	else
	{
		return NULL;
	}
}

/**
 * @brief Releases the binary semaphore
 *
 */
void DataStruct_ReleaseSemaphore(void)
{
	osSemaphoreRelease(binarySemaphoreHandle);
}
