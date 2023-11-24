/**
 * @file time.h
 * @author HEKA
 * @brief Header file for the time managing module of the firmware
 *
 * @date 2023-10-26
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "stdint.h"
#include "stdbool.h"

/* Constants */
#define TASK_DELAY_BSP_MS 30
#define TASK_DELAY_UART_MS 50

/* Function prototypes */
uint32_t Time_GetTimeMs(void);
bool Time_IsTimeUp(uint32_t last_task_call_ms, uint32_t compare_time_ms);
