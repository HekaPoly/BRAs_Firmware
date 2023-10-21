/**
 * @file uart.c
 * @author HEKA
 * @brief UART communication with the interface
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "../Inc/uart.h"
#include "main.h"

/* Global variables */
struct UART_t g_uart =
{
	.is_uart_initialized = false,
};

/**
 * @brief Initialization of the UART communication with the interface
 * 
 */
void UART_Init(void)
{
    /* Initialize the UART communication */
	g_uart.uart_handle = &huart2;
	g_uart.message_received = 0;
	g_uart.message_to_send = 30;
}

/**
 * @brief Communication task
 * 
 */
void UART_Task(void)
{
    /* Receive data from the interface */
	HAL_UART_Receive(g_uart.uart_handle,(uint8_t *) &g_uart.message_received, 4, 1000);
	HAL_Delay(100);
	g_uart.message_to_send= g_uart.message_received;
	HAL_UART_Transmit(g_uart.uart_handle,(uint8_t *) &g_uart.message_to_send, sizeof(g_uart.message_to_send), 1000);
	HAL_Delay(100);
    /* Update the data structure with desired speed and angle for every motor */
}
