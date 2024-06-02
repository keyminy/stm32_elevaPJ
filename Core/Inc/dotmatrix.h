/*
 * dotmatrix.h
 *
 *  Created on: Jun 1, 2024
 *      Author: me
 */

#ifndef INC_DOTMATRIX_H_
#define INC_DOTMATRIX_H_
#include "main.h"

void dotmatrix_main_test();
int dotmatrix_main(void);
void init_dotmatrix(void);
void set_dotmatrix_buffer(uint8_t curr_eleva_state, uint8_t target_floor);
void control_dotmatrix(uint8_t curr_eleva_state);
void clear_dotmatrix_buffer(void);


#endif /* INC_DOTMATRIX_H_ */
