/*
 * servo_motor.h
 *
 *  Created on: May 30, 2024
 *      Author: HARMAN-27
 */

#ifndef INC_SERVO_MOTOR_H_
#define INC_SERVO_MOTOR_H_

#include "main.h"
void temp_servo_motor_run(void);
void set_rpm(int rpm);
void stepmotor_main(void);
void stepmotor_drive(int step);
void stop_motor(void);
void perform_motor_step_forward();
void perform_motor_step_backward();
void ctrl_stepmotor_eleva(uint8_t curr_eleva_state);
#endif /* INC_SERVO_MOTOR_H_ */
