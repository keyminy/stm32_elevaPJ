/*
 * stepmotor.h
 *
 *  Created on: Jun 3, 2024
 *      Author: HARMAN-27
 */

#ifndef INC_STEPMOTOR_H_
#define INC_STEPMOTOR_H_
#include "main.h"

void set_rpm(int rpm);
void perform_motor_step_forward();
void perform_motor_step_backward();
void stepmotor_drive(int step);
void stop_motor(void);
void ctrl_stepmotor_eleva(uint8_t curr_eleva_state);

#endif /* INC_STEPMOTOR_H_ */
