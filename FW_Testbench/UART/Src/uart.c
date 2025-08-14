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
#include "stdio.h"

/* Private functions declaration */
static void Receive_Data(UART * uart);
static void Transmit_Data(UART * uart, Data * data_structure);

/* Global variables */
uint8_t g_rx_buffer[NUMBER_OF_BYTES_PER_MSG] = {0};
char buffer[100];

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
		else
		{
			//Transmit_Data(&g_uart, data_structure);
			sprintf(g_tx_buffer, "Encoder Ticks = %f\r\n", data_structure->encoder_base.encoder_degrees);
			HAL_UART_Transmit(g_uart.uart_handle, g_tx_buffer, sizeof(g_tx_buffer), 1000);

			memset(g_tx_buffer, 0, 50);
			sprintf(g_tx_buffer, "\r\n");
			HAL_UART_Transmit(g_uart.uart_handle, g_tx_buffer, sizeof(g_tx_buffer), 1000);

		 // Debugging - Print received data
		char buffer[100];
	
		 // Reconstruct 32-bit data (little-endian)
		 uint32_t received_data = g_uart.message_received[0] |
		 ((uint32_t)g_uart.message_received[1] << 8) |
		 ((uint32_t)g_uart.message_received[2] << 16) |
		 ((uint32_t)g_uart.message_received[3] << 24);

		// Extract data
		uint8_t motor_id = received_data & 0x07;          // First 3 bits
		uint16_t velocity = (received_data >> 3) & 0x1FFF;    // Next 13 bits
		uint16_t angle = (received_data >> 16) & 0xFFFF;      // Upper 16 bits
	
		// debug putty
		// sprintf(buffer, "Extracted Values: Motor ID = %d, Velocity = %d, Angle = %d\n",
		// 	motor_id, velocity, angle);
		// HAL_UART_Transmit(g_uart.uart_handle, (uint8_t *)buffer, strlen(buffer), 1000);
	
		// Validate motor ID
		if (motor_id >= NUMBER_MOTOR)
		{
			return;  // Ignore invalid motor ID
		}
	
		// Update motor data
		data_structure->Data_Motors[motor_id].motor_desired_speed_percent = velocity;
		switch(motor_id)
		{
			case 0:
			{
				data_structure->Data_Motors[motor_id].motor_angle_to_reach_deg = angle /3;
				break;
			}
			case 1:
			{
				data_structure->Data_Motors[motor_id].motor_angle_to_reach_deg = angle /8;
				break;
			}
			case 2:
			{
				data_structure->Data_Motors[motor_id].motor_angle_to_reach_deg = angle*10;
				break;
			}

			default:

				data_structure->Data_Motors[motor_id].motor_angle_to_reach_deg = angle;
				break;
		}

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
