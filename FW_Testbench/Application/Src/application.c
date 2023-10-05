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
#include "../../BSP/Inc/uart.h"
#include "../../BSP/Inc/bsp.h"

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
    /* Call UART_Task() */
    /* Call BSP_Task() */
}
