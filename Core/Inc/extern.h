/*
 * extern.h
 *
 *  Created on: Jun 1, 2024
 *      Author: me
 */

#ifndef INC_EXTERN_H_
#define INC_EXTERN_H_

extern void led_all_on(void);
extern void led_all_off(void);
extern void led2_toggle(void);
extern void led_on_up();
extern void led_on_down();
extern void ledbar0_toggle(void);
extern void ledbar1_toggle(void);
extern void ledbar2_toggle(void);
extern void led_main(void);
extern void button0_check(void);
extern void delay_us(unsigned int us);
extern void get_rtc_date_time(void);
extern void pc_command_processing(void);
extern void led_74hc595_demo(void);
extern void dotmatrix_main_test(void);
extern void stepmotor_main(void);
// Controls the motor state based on the state of the button being pressed.
extern void stepmotor_btn_check(void);
extern void dc_motor_pwm_control(void);
extern void UpdateLEDBasedOnPWM();

#endif /* INC_EXTERN_H_ */
