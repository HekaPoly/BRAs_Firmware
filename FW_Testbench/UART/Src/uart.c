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

#include "uart.h"
#include "main.h"
#include "data_structure.h"

/* Private functions declaration */
static void Receive_Data(UART * uart);
static void Transmit_Data(UART * uart, Data * data_structure);

/* Global variables */
uint8_t g_rx_buffer[NUMBER_OF_BYTES_PER_MSG] = {0};
uint8_t g_tx_buffer[50] = {0};

UART g_uart =
{
	.is_uart_initialized = false,
	.uart_handle = 0u,
	.message_received = g_rx_buffer,
};

/**
 * @brief Initialization of the UART communication with the interface
 * 
 */
void UART_Init(void)
{
    /* Initialize the UART communication */
	g_uart.uart_handle = &huart2;

	g_uart.is_uart_initialized = true;
}

/**
 * @brief Recieve data and update the appropriate data structure fields
 * 
 */
void UART_Task(void)
{
	if (g_uart.is_uart_initialized)
	{
		Receive_Data(&g_uart);

		/* Update the data structure approprietly */
		Data * data_structure = DataStruct_Get();
		if (data_structure == NULL)
		{
			//return MOTOR_STATE_WAITING_FOR_SEMAPHORE;
		}
		else
		{
			data_structure->motor_base.motor_desired_speed_percent = (g_uart.message_received[INDEX_FIRST_BYTE] + (g_uart.message_received[INDEX_SECOND_BYTE] << 8));
			data_structure->motor_base.motor_angle_to_reach_deg = (g_uart.message_received[INDEX_THIRD_BYTE] + (g_uart.message_received[INDEX_FOURTH_BYTE] << 8));

			//Transmit_Data(&g_uart, data_structure);
			sprintf(g_tx_buffer, "Encoder Ticks = %f\r\n", data_structure->encoder_base.encoder_degrees);
			HAL_UART_Transmit(g_uart.uart_handle, g_tx_buffer, sizeof(g_tx_buffer), 1000);

			memset(g_tx_buffer, 0, 50);
			sprintf(g_tx_buffer, "\r\n");
			HAL_UART_Transmit(g_uart.uart_handle, g_tx_buffer, sizeof(g_tx_buffer), 1000);

			DataStruct_ReleaseSemaphore();
		}
	}
}

/**
 * @brief Data reception
 * 
 * @param[in] uart The UART structure containing pertinent information about the UART module used
 */
static void Receive_Data(UART * uart)
{
	HAL_UART_Receive(uart->uart_handle, g_rx_buffer, NUMBER_OF_BYTES_PER_MSG, 1000);
}

static void Transmit_Data(UART * uart, Data * data_structure)
{
	//HAL_UART_Transmit(uart->uart_handle, ((uint32_t) data_structure->encoder_base.encoder_degrees), NUMBER_OF_BYTES_PER_MSG, 1000);
}
