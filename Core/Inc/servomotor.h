/*
 * servo_motor.h
 *
 *  Created on: May 30, 2024
 *      Author: HARMAN-27
 */

#ifndef INC_SERVOMOTOR_H_
#define INC_SERVOMOTOR_H_

#include "main.h"

void temp_servo_motor_run(void);
void servo_motor_main(void);
void set_servo_state(uint8_t p_servostate);
uint8_t get_servo_state(void);
void init_servo_LOCKED(void);

#endif /* INC_SERVOMOTOR_H_ */
