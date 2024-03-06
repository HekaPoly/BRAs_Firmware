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

/* Constants */
#define FACTOR_SECONDS_TO_MS 1000
#define NUMBER_MOTOR 6

/* Private functions declaration */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired);
static void Modify_Direction(int16_t difference_deg);

/* Global variables */
Motor Motors[NUMBER_MOTOR]; //Array of all the motors

Data_motor currentData; 	//Data of the Motor we are currently changing
Motor* currentMotor;      	//structure of the motor we are currently changing

/* Function implementation */

/**
 * @brief Initialization function for all stepper motors
 * 
 */
void MotorControl_Init(void)
{
    /* Initialize all stepper motors (PWM, Direction GPIO and Enable GPIO) */
	for(i=0;i<NUMBER_MOTOR;i++){ //need to change the timing channels for each individual motors (use case statement)
		Motors[i].motor_direction = MOTOR_DIRECTION_CLOCKWISE;
		Motors[i].motor_timer_handle = &htim2;
		Motors[i].motor_timer_channel = TIM_CHANNEL_1;
		Motors[i].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX;
		Motors[i].nb_pulse = 0u;
		Motors[i].delay = 0u;

		Motors[i].is_motor_initialized = true;
	}
}

/**
 * @brief Task to control the stepper motors in manual mode
 * 
 */
Motor_State MotorControl_Task(void)
{
	Data * data_structure = DataStruct_Get();

	if (data_structure == NULL){
		return MOTOR_STATE_WAITING_FOR_SEMAPHORE;
	}

	for(i=0;i<NUMBER_MOTOR;i++){ //loops each motor

		//call encoder

		//call PID

		currentMotor = &Motors[i];

		switch(i){ //gets the degree difference for the specific motor
			case 0:
				currentData = data_structure->Motor_0;
				break;
			case 1:
				currentData = data_structure->Motor_1;
				break;
			case 2:
				currentData = data_structure->Motor_2;
				break;
			case 3:
				currentData = data_structure->Motor_3;
				break;
			case 4:
				currentData = data_structure->Motor_4;
				break;
			case 5:
				currentData = data_structure->Motor_5;
				break;
			default :
				/*nothing*/
				break;
		}

		int16_t difference_deg = currentData.motor_angle_to_reach_deg - currentData.motor_current_angle_deg;

		if (difference_deg != 0){
			Modify_Direction(difference_deg);
			Modify_Speed(difference_deg, currentData.motor_desired_speed_percent);

			currentData.motor_current_angle_deg = currentData.motor_angle_to_reach_deg;
		}
	}

	DataStruct_ReleaseSemaphore();

	return MOTOR_STATE_OK;
}

/**
 * @brief Calculation of new ARR value to give the PWM in order to modifiy the motor's speed
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 *
 */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired_percent)
{
	uint16_t new_freq = (motor_speed_desired_percent * FREQ_MAX_HZ) / 100;
	uint16_t new_arr = (FREQ_CLK_HZ / ((PSC + 1)* new_freq)) - 1;

	currentMotor->motor_timer_handle->Instance->ARR = new_arr;
	currentMotor->motor_timer_handle->Instance->CCR1 = currentMotor->motor_timer_handle->Instance->ARR / 2;

	currentMotor->nb_pulse = abs(difference_deg) / currentMotor->deg_per_turn;
	currentMotor->delay = ((float)currentMotor->nb_pulse / (float)new_freq) * FACTOR_SECONDS_TO_MS;

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //add modularity to the pin

	HAL_TIM_PWM_Start(currentMotor->motor_timer_handle, currentMotor->motor_timer_channel);
	HAL_Delay(currentMotor->delay);
	HAL_TIM_PWM_Stop(currentMotor->motor_timer_handle, currentMotor->motor_timer_channel);

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //add modularity to the pin
}

/**
 * @brief Modifies the direction signal controlling the turning direction of the motor
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 *
 */
static void Modify_Direction(int16_t difference_deg)
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
		HAL_GPIO_WritePin(DIR_Motor_1_GPIO_Port, DIR_Motor_1_Pin, GPIO_PIN_SET);  // check validity of the pin and motor pin number member
	}
    else if (currentMotor->motor_direction == MOTOR_DIRECTION_COUNTERCLOCKWISE)
	{
    	HAL_GPIO_WritePin(DIR_Motor_1_GPIO_Port, DIR_Motor_1_Pin, GPIO_PIN_RESET); // check validity of the pin and motor pin number member
	}
}
