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

/* Global variables */
uint8_t g_rx_buffer[NUMBER_OF_BYTES_PER_MSG] = {0};


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

	// Ready to receive the first message
	Receive_Data(&g_uart);

}

/**
 * @brief This function is responsible for processing UART data and updating the data structure.
 * 
 * It receives data from the UART and updates the data structure with the received values.
 * The received data is distributed to all motors in the data structure.
 * 
 * @note This function assumes that the UART is already initialized.
 */
void UART_Task(void)
{

	if (g_uart.is_uart_initialized){

		/* Update the data structure appropriately */
		Data *data_structure = DataStruct_Get();

		if (data_structure == NULL)
		{
			//return MOTOR_STATE_WAITING_FOR_SEMAPHORE;
		}


		// Distribute the same data to all motors

		data_structure->Data_Motors[0].motor_desired_speed_percent = (g_uart.message_received[INDEX_FIRST_BYTE] + (g_uart.message_received[INDEX_SECOND_BYTE] << 8));
		data_structure->Data_Motors[0].motor_angle_to_reach_deg = (g_uart.message_received[INDEX_THIRD_BYTE] + (g_uart.message_received[INDEX_FOURTH_BYTE] << 8));

		data_structure->Data_Motors[1].motor_desired_speed_percent = (g_uart.message_received[INDEX_FIRST_BYTE] + (g_uart.message_received[INDEX_SECOND_BYTE] << 8));
		data_structure->Data_Motors[1].motor_angle_to_reach_deg = (g_uart.message_received[INDEX_THIRD_BYTE] + (g_uart.message_received[INDEX_FOURTH_BYTE] << 8));

		data_structure->Data_Motors[2].motor_desired_speed_percent = (g_uart.message_received[INDEX_FIRST_BYTE] + (g_uart.message_received[INDEX_SECOND_BYTE] << 8));
		data_structure->Data_Motors[2].motor_angle_to_reach_deg = (g_uart.message_received[INDEX_THIRD_BYTE] + (g_uart.message_received[INDEX_FOURTH_BYTE] << 8));
		/*
		data_structure->Data_Motors[3].motor_desired_speed_percent = (g_uart.message_received[INDEX_FIRST_BYTE] + (g_uart.message_received[INDEX_SECOND_BYTE] << 8));
		data_structure->Data_Motors[3].motor_angle_to_reach_deg = (g_uart.message_received[INDEX_THIRD_BYTE] + (g_uart.message_received[INDEX_FOURTH_BYTE] << 8));

		data_structure->Data_Motors[4].motor_desired_speed_percent = (g_uart.message_received[INDEX_FIRST_BYTE] + (g_uart.message_received[INDEX_SECOND_BYTE] << 8));
		data_structure->Data_Motors[4].motor_angle_to_reach_deg = (g_uart.message_received[INDEX_THIRD_BYTE] + (g_uart.message_received[INDEX_FOURTH_BYTE] << 8));

		data_structure->Data_Motors[5].motor_desired_speed_percent = (g_uart.message_received[INDEX_FIRST_BYTE] + (g_uart.message_received[INDEX_SECOND_BYTE] << 8));
		data_structure->Data_Motors[5].motor_angle_to_reach_deg = (g_uart.message_received[INDEX_THIRD_BYTE] + (g_uart.message_received[INDEX_FOURTH_BYTE] << 8));
		*/
		DataStruct_ReleaseSemaphore();

	}
}


/**
 * @brief Data reception
 *
 * @param[in] uart The UART structure containing pertinent information about the UART module used
 */
static void Receive_Data(UART * uart)
{
	 HAL_UART_Receive_IT(uart->uart_handle, g_rx_buffer, NUMBER_OF_BYTES_PER_MSG);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

{

	  if (huart == g_uart.uart_handle)
	    {
	        // Re-arm reception for the next 4-byte packet
	        Receive_Data(&g_uart);
	    }
}


