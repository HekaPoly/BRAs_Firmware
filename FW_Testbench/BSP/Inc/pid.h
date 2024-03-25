/**
 * @file pid.h
 * @author HEKA
 * @brief Header file for the PID implementation
 *
 * @date 2024-02-29
 * 
 * @copyright Copyright HEKA (c) 2024
 * 
 */

#ifndef __PID_H__
#define __PID_H__

#include "motor_control.h"

/* Constants */

/* Structs and enums */

/* Function declaration */
void Pid_Algorithm(int difference_deg, Motor * current_motor);

#endif /* __PID_H__ */
