/**
 * @file motor_control.c
 * @author HEKA
 * @brief Manual motor control implementation
 *
 * @date 2023-10-05
 * 
 * @copyright Copyright HEKA (c) 2023
 * 
 */

#include "motor_control.h"
#include "main.h"
#include "data_structure.h"

#include "encoder.h"
#include <stdlib.h>
#include <time.h>

/* Constants */
#define FACTOR_SECONDS_TO_MS 1000
#define NUMBER_MOTOR 6


/* Private functions declaration */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired, Motor* currentMotor);
static void Modify_Direction(int16_t difference_deg, Motor* currentMotor);

/* Global variables */
Motor Motors[NUMBER_MOTOR]; //Array of all the motors

/* Function implementation */

/**
 * @brief Initialization function for all stepper motors
 * 
 */
void MotorControl_Init(void)
{
    // Initializes all stepper motors (PWM, Direction GPIO and Enable GPIO)
	for(int i=0;i<NUMBER_MOTOR;i++){
		Motors[i].motor_direction = MOTOR_DIRECTION_CLOCKWISE;
		Motors[i].nb_pulse = 0u;
		Motors[i].delay = 0u;
		Motors[i].is_motor_initialized = true;
	}
	// Initialize the timer and the channels for each motor
	// change timing handle if necessary

	Motors[0].motor_timer_handle = &htim1;
	Motors[0].motor_timer_channel = TIM_CHANNEL_1;
	Motor_gpio_section direction0 = {DIRECTION_MOTOR_0_GPIO_PORT, DIRECTION_MOTOR_0_PIN};
	Motors[0].direction_port = direction0;
	Motors[0].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX_0;

	Motors[1].motor_timer_handle = &htim1;
	Motors[1].motor_timer_channel = TIM_CHANNEL_2;
	Motor_gpio_section direction1 = {DIRECTION_MOTOR_1_GPIO_PORT, DIRECTION_MOTOR_1_PIN};
	Motors[1].direction_port = direction1;
	Motors[1].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX_1;



	Motors[2].motor_timer_handle = &htim1;
	Motors[2].motor_timer_channel = TIM_CHANNEL_3;
	Motor_gpio_section direction2 = {DIRECTION_MOTOR_2_GPIO_PORT, DIRECTION_MOTOR_2_PIN};
	Motors[2].direction_port = direction2;
	Motors[2].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX_2;


	Motors[3].motor_timer_handle = &htim1;
	Motors[3].motor_timer_channel = TIM_CHANNEL_4;
	Motor_gpio_section direction3 = {DIRECTION_MOTOR_3_GPIO_PORT, DIRECTION_MOTOR_3_PIN};
	Motors[3].direction_port = direction3;
	Motors[3].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX_3;

	Motors[4].motor_timer_handle = &htim2;
	Motors[4].motor_timer_channel = TIM_CHANNEL_1;
	Motor_gpio_section direction4 = {DIRECTION_MOTOR_4_GPIO_PORT, DIRECTION_MOTOR_4_PIN};
	Motors[4].direction_port = direction4;
	Motors[4].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX_4;

	Motors[5].motor_timer_handle = &htim2;
	Motors[5].motor_timer_channel = TIM_CHANNEL_3;
	Motor_gpio_section direction5 = {DIRECTION_MOTOR_5_GPIO_PORT, DIRECTION_MOTOR_5_PIN};
	Motors[5].direction_port = direction5;
	Motors[5].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX_5;

}

/**
 * @brief Task to control the stepper motors in manual mode
 * @return The state of the motors.
 */
Motor_State MotorControl_Task(void)
{
	Data * data_structure = DataStruct_Get();
	
	if (data_structure == NULL) return MOTOR_STATE_WAITING_FOR_SEMAPHORE;
	
	// Data of the Motor we are currently changing
	Data_Motor* currentData; 
	Motor* currentMotor;


	// Modified code to only loop through one motors; add more motors progressively
	for(int i=0;i<NUMBER_MOTOR-5;i++){ // loops each motor
		HAL_Delay(2000);

		// call encoder

		// call PID
		
		currentData = &data_structure->Data_Motors[i];
		currentMotor = &Motors[i];

		int16_t difference_deg = currentData->motor_angle_to_reach_deg - currentData->motor_current_angle_deg;

		if (difference_deg != 0){
			Modify_Direction(difference_deg, currentMotor);
			Modify_Speed(difference_deg, currentData->motor_desired_speed_percent, currentMotor);

			currentData->motor_current_angle_deg = currentData->motor_angle_to_reach_deg;
		}
	}

	DataStruct_ReleaseSemaphore();

	return MOTOR_STATE_OK;
}

/**
 * @brief Modifies the speed of a motor based on the desired speed percentage and the difference in degrees.
 *
 * This function calculates the new frequency and new auto-reload value (ARR) for the motor timer based on the desired speed percentage.
 * It then updates the ARR and CCR1 registers of the motor timer to control the motor speed.
 * The number of pulses and delay are calculated based on the difference in degrees and the new frequency.
 * The LED is toggled on and off to indicate the motor operation.
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 * @param[in] motor_speed_desired_percent	The desired motor speed percentage used to calculate the motor speed
 * @param[in] currentMotor	The current motor's speed we are changing
 */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired_percent, Motor* currentMotor)
{
    // Calculate new frequency and ARR based on desired speed percentage
    uint16_t new_freq = (motor_speed_desired_percent * FREQ_MAX_HZ) / 100;
    uint16_t new_arr = (FREQ_CLK_HZ / ((PSC + 1) * new_freq)) - 1;

    // Update the ARR for the timer
    __HAL_TIM_SET_AUTORELOAD(currentMotor->motor_timer_handle, new_arr);

    // Update the duty cycle (CCR) using the appropriate channel macro
    uint16_t duty_cycle = new_arr / 2; // 50% duty cycle as an example
    __HAL_TIM_SET_COMPARE(currentMotor->motor_timer_handle, currentMotor->motor_timer_channel, duty_cycle);

    // Calculate number of pulses and delay based on the difference in degrees
    currentMotor->nb_pulse = abs(difference_deg) / currentMotor->deg_per_turn;
    currentMotor->delay = ((float)currentMotor->nb_pulse / (float)new_freq) * FACTOR_SECONDS_TO_MS;

    // Toggle LED to indicate motor operation
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); // Turn on LED

    // Start PWM on the appropriate channel
    HAL_TIM_PWM_Start(currentMotor->motor_timer_handle, currentMotor->motor_timer_channel);

    // Delay for motor operation
    HAL_Delay(currentMotor->delay);

    // Stop PWM on the appropriate channel
    HAL_TIM_PWM_Stop(currentMotor->motor_timer_handle, currentMotor->motor_timer_channel);

    // Toggle LED to indicate motor operation ended
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); // Turn off LED
}

/**
 * @brief Modifies the direction signal of a motor based on the given difference in degrees.
 *
 * This function updates the direction of the motor based on the difference in degrees.
 * If the difference is negative, the motor direction is set to counter-clockwise.
 * If the difference is positive, the motor direction is set to clockwise.
 * The direction is then set on the motor driver using the appropriate GPIO pin.
 *
 * @param[in] difference_deg The difference in degrees to determine the motor direction.
 * @param[in] currentMotor Pointer to the Motor structure representing the motor.
 */
static void Modify_Direction(int16_t difference_deg , Motor* currentMotor)
{
	//switch direction if needed
	if (difference_deg < 0)
	{
		currentMotor->motor_direction = MOTOR_DIRECTION_COUNTERCLOCKWISE;
	}
	else if (difference_deg > 0)
	{
		currentMotor->motor_direction = MOTOR_DIRECTION_CLOCKWISE;
	}

	//set the direction on the driver
	if (currentMotor->motor_direction == MOTOR_DIRECTION_CLOCKWISE)
	{
		HAL_GPIO_WritePin(currentMotor->direction_port.section, currentMotor->direction_port.pin_num, GPIO_PIN_SET);  // check validity of the pin and motor pin number member
	}
	else if (currentMotor->motor_direction == MOTOR_DIRECTION_COUNTERCLOCKWISE)
	{
		HAL_GPIO_WritePin(currentMotor->direction_port.section, currentMotor->direction_port.pin_num, GPIO_PIN_RESET); // check validity of the pin and motor pin number member
	}
}
