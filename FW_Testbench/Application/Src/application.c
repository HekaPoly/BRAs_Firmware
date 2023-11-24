/**
 * @file application.c
 * @author HEKA
 * @brief Main entry point of the manual control firmware
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "../Inc/application.h"
#include "../../UART/Inc/uart.h"
#include "../../BSP/Inc/bsp.h"
#include "../Inc/time.h"

/* Function implementation */
/**
 * @brief Initialization of the different modules of the firmware
 * 
 */
void Application_Init(void)
{
    UART_Init();
    BSP_Init();
}

/**
 * @brief Main loop
 * 
 */
void Application_Task(void)
{
	uint32_t uart_task_time_ms = 0;
	uint32_t bsp_task_time_ms = 0;
	
	while(1)
	{
		if (Time_IsTimeUp(uart_task_time_ms, TASK_DELAY_UART_MS))
		{
			UART_Task();
			uart_task_time_ms = Time_GetTimeMs();
		}

		if (Time_IsTimeUp(bsp_task_time_ms, TASK_DELAY_BSP_MS))
		{
			BSP_Task();
			bsp_task_time_ms = Time_GetTimeMs();
		}

		/* Call BSP_Task() */
	}

}
