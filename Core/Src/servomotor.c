#include <servomotor.h>
#include "def.h"
#include "extern.h"

extern TIM_HandleTypeDef htim3;
extern volatile int TIM2_servo_open_time;// this is 1ms count variable

uint8_t temp_servo_state = 0;


/*
 * TIM3 operating frequency : 84MHz
 * prescaler : 1680 -> TIM3 : 50KHz
 * Counter Period : 1000-1
 * frequency of PWM signal : 50Hz
 * T = 20ms
 * Servomotor 180 degrees : 2ms duty
 * Servomotor 90 degrees : 1.5ms duty
 * Servomotor 0 degrees : 1ms duty
 * */


void servo_motor_main(void) {
	static int servo_elevator_state=0;
	//static int off_flag=0;

	if(open_flag==1){
		switch (servo_elevator_state) {
		case 0:
			// 1.rotation of 90 dgrees
//		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 75);
			TIM2_servo_open_time = 0;
			servo_elevator_state = 1;
			break;
		case 1:
			if(TIM2_servo_open_time >=3000){
				open_state = DOOR_CLOSE;
				open_flag = 0;
			}
			// 1.rotation of 0 dgrees
//		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 28);
			break;
		}

	}
}


void temp_servo_motor_run(void) {
		if(TIM2_servo_open_time == 5000){
			temp_servo_state = 2;
		}
		// every 1sec
		/* rotation order of dgrees : 180 --> 90 --> 0 */
		switch (temp_servo_state) {
		case 0:
			// 1.rotation of 180 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 126); // 2ms
			break;
		case 1:
			// 1.rotation of 90 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 75);
			HAL_GPIO_WritePin(GPIOB, 0x0F, GPIO_PIN_SET);
			break;
		case 2:
			// 1.rotation of 0 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 28);
			HAL_GPIO_WritePin(GPIOB, 0x0F, GPIO_PIN_RESET);
			break;
		}

}
