﻿/*
 * button.h
 *
 * Created: 2024-04-25 오후 1:15:17
 *  Author: kccistc
 */ 
#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_
#include "main.h"   // for GPIO Handling

#define BUTTON0   0
#define BUTTON1   1
#define BUTTON2   2
#define BUTTON3   3

#define BUTTON_PRESS 0   // 버튼을 누르면 high  active-low
#define BUTTON_RELEASE 1  // 버튼을 떼면 low
#define BUTTON_NUMBER 4   // 버튼 갯수

int get_button(GPIO_TypeDef  *GPIO, int button_pin, int button_num);
void button0_check(void);
// Controls the motor state based on the state of the button being pressed.
void stepmotor_btn_check(void);
void choose_eleva_floor(void);

#endif
