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
    /* Initialize all stepper motors (PWM, Direction GPIO and Enable GPIO) */
	for(int i=0;i<NUMBER_MOTOR;i++){ //need to change the timing channels for each individual motors (use case statement)
		Motors[i].motor_direction = MOTOR_DIRECTION_CLOCKWISE;
		Motors[i].deg_per_turn = DEGREES_PER_PULSE_WITH_GEARBOX;
		Motors[i].nb_pulse = 0u;
		Motors[i].delay = 0u;
		Motors[i].is_motor_initialized = true;
	}

	/* Initialize the timer and the channels for each motor */

	Motors[0].motor_timer_handle = &htim1;		   		//TODO :need to change the timing handle for each individual motors
	Motors[0].motor_timer_channel = TIM_CHANNEL_1; 		//TODO :need to change the timing channels for each individual motors

	gpio_section direction0 = {DIR_Motor_0_GPIO_Port, DIR_Motor_0_Pin};
	
	Motors[0].direction_port = direction0;   


	Motors[1].motor_timer_handle = &htim1;	   			//TODO :need to change the timing handle for each individual motors
	Motors[1].motor_timer_channel = TIM_CHANNEL_2;	   	//TODO :need to change the timing channels for each individual motors
	
	gpio_section direction1 = {DIR_Motor_1_GPIO_Port, DIR_Motor_1_Pin};

	Motors[1].direction_port = direction1;
	

	Motors[2].motor_timer_handle = &htim1; 	   			//TODO :need to change the timing handle for each individual motors
	Motors[2].motor_timer_channel = TIM_CHANNEL_3;	   	//TODO :need to change the timing channels for each individual motors
	
	gpio_section direction2 = {DIR_Motor_2_GPIO_Port, DIR_Motor_2_Pin};
	
	Motors[2].direction_port = direction2;


	Motors[3].motor_timer_handle = &htim1;	   			//TODO :need to change the timing handle for each individual motors
	Motors[3].motor_timer_channel = TIM_CHANNEL_4;	   	//TODO :need to change the timing channels for each individual motors
	
	gpio_section direction3 = {DIR_Motor_3_GPIO_Port, DIR_Motor_3_Pin};

	Motors[3].direction_port = direction3;
	

	Motors[4].motor_timer_handle = &htim2;	   			//TODO :need to change the timing handle for each individual motors
	Motors[4].motor_timer_channel = TIM_CHANNEL_1;	   	//TODO :need to change the timing channels for each individual motors
	
	gpio_section direction4 = {DIR_Motor_4_GPIO_Port, DIR_Motor_4_Pin};
	
	Motors[4].direction_port = direction4;
	
	
	Motors[5].motor_timer_handle = &htim2;	   			//TODO :need to change the timing handle for each individual motors
	Motors[5].motor_timer_channel = TIM_CHANNEL_3;	   	//TODO :need to change the timing channels for each individual motors
	
	gpio_section direction5 = {DIR_Motor_5_GPIO_Port, DIR_Motor_5_Pin};
	
	Motors[5].direction_port = direction5;
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
	
	Data_Motor* currentData; //Data of the Motor we are currently changing

	Motor* currentMotor;

	for(int i=0;i<NUMBER_MOTOR;i++){ //loops each motor

		//call encoder

		//call PID
		
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
 * @brief Calculation of new ARR value to give the PWM in order to modifiy the motor's speed
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 *
 */
static void Modify_Speed(int16_t difference_deg, uint32_t motor_speed_desired_percent, Motor* currentMotor)
{
	uint16_t new_freq = (motor_speed_desired_percent * FREQ_MAX_HZ) / 100;
	uint16_t new_arr = (FREQ_CLK_HZ / ((PSC + 1)* new_freq)) - 1;

	currentMotor->motor_timer_handle->Instance->ARR = new_arr;
	currentMotor->motor_timer_handle->Instance->CCR1 = currentMotor->motor_timer_handle->Instance->ARR / 2;

	currentMotor->nb_pulse = abs(difference_deg) / currentMotor->deg_per_turn;
	currentMotor->delay = ((float)currentMotor->nb_pulse / (float)new_freq) * FACTOR_SECONDS_TO_MS;

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //Fonction?

	HAL_TIM_PWM_Start(currentMotor->motor_timer_handle, currentMotor->motor_timer_channel);
	HAL_Delay(currentMotor->delay);
	HAL_TIM_PWM_Stop(currentMotor->motor_timer_handle, currentMotor->motor_timer_channel);

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //Fonction?
}

/**
 * @brief Modifies the direction signal controlling the turning direction of the motor
 *
 * @param[in] difference_deg	The difference between the angle to reach and the current motor's angle
 *
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
