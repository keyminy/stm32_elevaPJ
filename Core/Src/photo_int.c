#include "photo_int.h"
#include "def.h"
#include "servo_motor.h"
#include "elevator.h"
#include "dotmatrix.h"

/**
 * moved from Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
 * EXTI Callback Function
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* Prevent unused argument(s) compilation warning */
	UNUSED(GPIO_Pin);
	/* NOTE: This function Should not be modified, when the callback is needed,
	 the HAL_GPIO_EXTI_Callback could be implemented in the user file
	 */
	switch (GPIO_Pin) {
	case PhotoSensor_EXTI0_Pin: // PA0 -> FLOOR_1
		printf("GPIO_PIN_0!!\n"); //for test
		set_curr_floor(FLOOR_1);
		display_lcd_floor_info();
		break;
	case PhotoSensor_EXTI1_Pin: // PA1 -> FLOOR_2
		printf("GPIO_PIN_1!!\n"); //for test
		set_curr_floor(FLOOR_2);
		display_lcd_floor_info();

		break;
	case PhotoSensor_EXTI4_Pin: // PA4 -> FLOOR_3
		printf("GPIO_PIN_4!!\n"); //for test
		set_curr_floor(FLOOR_3);
		display_lcd_floor_info();

		break;
	}

	if(get_curr_floor() == get_target_floor()){
		// Stop step motor, Because we've reached the desired floor.
		set_curr_eleva_state(ELEVA_STOP);
		// clear_dotmatrix_buff();
	}else{
		// display dotmatrixLED to show the floor currently facing and the driection of the arrow.
		switch(get_curr_eleva_state()){
		case ELEVA_START_BOTTOM_UP:
			set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor()+1);
			break;
		case ELEVA_START_TOP_DOWN:
			set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor()-1);
			break;
		}
	}
}
