/**
 * @file time.c
 * @author HEKA
 * @brief Time management module
 *
 * @date 2023-10-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../Inc/time.h"
#include "stm32f4xx_hal.h"

/* Function implementation */
/**
 * @brief Gets the elapsed time in milliseconds since the uC was powered on
 * 
 * @return uint32_t 
 */
uint32_t Time_GetTimeMs(void)
{
    return HAL_GetTick();
}

/**
 * @brief Compares the elapsed time since a task was executed
 * 
 * @param[in] last_task_call_ms Time in ms when the concerned task was called 
 * @param[in] compare_time_ms Time in ms that has to be elapsed for the task to be called again
 * @return True if the elapsed time is longer than the compared time, false otherwise
 */
bool Time_IsTimeUp(uint32_t last_task_call_ms, uint32_t compare_time_ms)
{
    if (Time_GetTimeMs() - last_task_call_ms > compare_time_ms)
    {
        return true;
    }
    else
    {
        return false;
    }
}
