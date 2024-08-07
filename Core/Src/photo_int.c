#include <servomotor.h>
#include "photo_int.h"
#include "def.h"
#include "elevator.h"
#include "dotmatrix.h"
#include "servomotor.h"
#include "extern.h"

extern volatile int TIM2_servo_motor_count;
/**
 * moved from Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
 * EXTI Callback Function
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case PhotoSensor_EXTI0_Pin: // PA0 -> FLOOR_1
//		printf("GPIO_PIN_0!!\n"); //for test
		current_floor = 1;
		break;
	case PhotoSensor_EXTI1_Pin: // PA1 -> FLOOR_2
//		printf("GPIO_PIN_1!!\n"); //for test
		current_floor = 2;
		break;
	case PhotoSensor_EXTI4_Pin: // PA4 -> FLOOR_3
//		printf("GPIO_PIN_4!!\n"); //for test
		current_floor = 3;
		break;
	case PhotoSensor_EXTI5_Pin: // PA5 -> FLOOR_4
//		printf("GPIO_PIN_5!!\n"); //for test
		current_floor = 4;
		break;
	}
	if(target_floor_arr[current_floor] == 1){
//		printf("g!!\n");
		target_floor_arr[current_floor] = 0;
		open_state = DOOR_OPEN;
		open_flag = 1;
		set_prev_eleva_state(get_curr_eleva_state());
		set_curr_eleva_state(ELEVA_STOP);
	}
//	if(get_curr_floor() == get_target_floor()){
//		// Stop step motor, Because we've reached the desired floor.
//		set_curr_eleva_state(ELEVA_STOP);
//		// start_servo control : door open and close
//		set_servo_state(SERVO_OPEN);
//		TIM2_servo_motor_count = 0;
//		// end_servo control : door open and close
//		clear_dotmatrix_buffer();
//		// display current floor on dotmatrix
//		 set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor());
//	}else{
//		// display dotmatrixLED to show the floor currently facing and the driection of the arrow.
//		switch(get_curr_eleva_state()){
//		case ELEVA_START_BOTTOM_UP:
//			set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor()+1);
//			break;
//		case ELEVA_START_TOP_DOWN:
//			set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor()-1);
//			break;
//		}
//	}
}
