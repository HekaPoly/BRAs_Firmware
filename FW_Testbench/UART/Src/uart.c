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
			
		 // Debugging - Print received data
		// sprintf(buffer, "Received Bytes: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
		// 	g_rx_buffer[0], g_rx_buffer[1], g_rx_buffer[2], g_rx_buffer[3], g_rx_buffer[4]);
		// HAL_UART_Transmit(g_uart.uart_handle, (uint8_t *)buffer, strlen(buffer), 1000);
	
	
		// Extract data
		uint8_t motor_id = g_rx_buffer[INDEX_FIRST_BYTE];  // First byte is the motor ID
		uint16_t velocity = (g_rx_buffer[INDEX_SECOND_BYTE] | (g_rx_buffer[INDEX_THIRD_BYTE] << 8)); // Second & third bytes
		uint16_t angle = (g_rx_buffer[INDEX_FOURTH_BYTE] | (g_rx_buffer[INDEX_FIFTH_BYTE] << 8)); // Fourth & fifth bytes

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
		data_structure->Data_Motors[motor_id].motor_angle_to_reach_deg = angle;

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


