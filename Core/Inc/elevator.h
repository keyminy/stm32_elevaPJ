/*
 * elevator.h
 *
 *  Created on: Jun 1, 2024
 *      Author: me
 */

#ifndef INC_ELEVATOR_H_
#define INC_ELEVATOR_H_

#include "main.h"

void set_curr_floor(uint8_t floor);
uint8_t get_curr_floor(void);
void set_target_floor(uint8_t floor);
uint8_t get_target_floor(void);
void set_curr_eleva_state(uint8_t eleva_state);
uint8_t get_curr_eleva_state(void);
void display_lcd_floor_info(void);
void display_lcd_alert_info(void);
void eleva_button_check(void);

#endif /* INC_ELEVATOR_H_ */
