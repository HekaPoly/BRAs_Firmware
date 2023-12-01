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

#include "../Inc/data_structure.h"
#include "cmsis_os.h"

/* External variables */
extern osSemaphoreId binarySemaphoreHandle;

/* Data structure address declaration in .c to keep other files from reaching it */
Data * data_struct = (Data *)0x20000300;

/* Function implementation */
/**
 * @brief Returns data structure address if the semaphore is not taken
 * 
 * @return The data structure address when the semaphore is available, else returns a null pointer
 */
Data * DataStruct_Get(void)
{
	if (osSemaphoreWait(binarySemaphoreHandle, 0) == osOK)
	{
	    return data_struct;
	}
	else
	{
		return NULL;
	}
}

/* Function implementation */
/**
 * @brief Releases the binary semaphore
 *
 */
void DataStruct_ReleaseSemaphore(void)
{
	/* No verifying of the release status - to be added if necessary */
	osSemaphoreRelease(binarySemaphoreHandle);
}
