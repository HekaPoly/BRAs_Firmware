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
#include "../Inc/motor_control.h"
#include "main.h"

/* External variables */
extern Motor g_base_motor;

/* Private functions declaration */
static void Receive_Data(UART * uart);

/* Global variables */
uint8_t g_rx_buffer[4] = {0};

struct UART_t g_uart =
{
	.is_uart_initialized = false,
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
}

/**
 * @brief Communication task
 * 
 */
void UART_Task(void)
{
    /* Receive data from the interface */
	Receive_Data(&g_uart);

	if (g_base_motor.motor_angle_to_reach == 20)
	{
		g_base_motor.motor_direction = DIRECTION_CLOCKWISE;
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	}
	if (g_base_motor.motor_angle_to_reach == 40)
	{
		g_base_motor.motor_direction = DIRECTION_COUNTERCLOCKWISE;
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	}

	/* Update the data structure with desired speed and angle for every motor */
}

/**
 * @brief Receives data from the GUI
 * 
 * @param uart The UART structure containing pertinent information about the UART module used
 */
static void Receive_Data(UART * uart)
{
	HAL_UART_Receive(uart->uart_handle, g_rx_buffer, 4, 1000);

	g_base_motor.motor_speed_percent = (uart->message_received[0] + (uart->message_received[1] << 8));
	g_base_motor.motor_angle_to_reach = (uart->message_received[2] + (uart->message_received[3] << 8));
}