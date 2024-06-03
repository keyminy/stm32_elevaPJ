/*
 * fnd.h
 *
 *  Created on: Jun 3, 2024
 *      Author: HARMAN-27
 */

#ifndef INC_FND_H_
#define INC_FND_H_

#include "main.h"
void init_fnd();
void fnd_display();
void fnd_latch_clock(void);
void fnd_shift_clock(void);
void fnd_send_data(uint8_t data);

#endif /* INC_FND_H_ */
