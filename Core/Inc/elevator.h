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
uint8_t isFloorEmpty(uint8_t *pArr,int size);
void choose_eleva_floor2(void);

#endif /* INC_ELEVATOR_H_ */
