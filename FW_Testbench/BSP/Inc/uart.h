/**
 * @file uart.h
 * @author HEKA
 * @brief Header file for UART communication with the interface
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "stdbool.h"
#include "usart.h"

/**
 * Structure describing the UART module
 */
typedef struct UART_t
{
	bool is_uart_initialized;
	UART_HandleTypeDef * uart_handle;
	uint32_t message_received;
	uint32_t message_to_send;

};
/* Function protoytpes */
void UART_Init(void);
void UART_Task(void);
