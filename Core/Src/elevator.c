#include "elevator.h"
#include "def.h"
#include "dotmatrix.h"
#include "button.h"
#include "i2c_lcd.h"
#include "button.h"

volatile static uint8_t curr_floor;
volatile static uint8_t target_floor;
volatile static uint8_t curr_eleva_state = ELEVA_STOP;
/* ref */
volatile uint8_t target_floor_arr[SIZE]={0,};
volatile uint8_t curr_floor_arr[SIZE]={0,1,0,0,0};
volatile uint8_t current_floor=0;
volatile char move_flag = 0;
volatile char open_flag = 0;
static uint8_t prev_eleva_state = ELEVA_STOP;
volatile DOOR_STATE open_state = DOOR_CLOSE;
/* end ref */

void elevator_init(void){
	/*initialize elevator state*/
	curr_floor = 1; // In the initial state, the elevator is on the first floor.
//	target_floor = FLOOR_1;
	current_floor = 1;

	curr_eleva_state = ELEVA_STOP;
	/*peripheral*/
//	i2c_lcd_init(); // warning LCD should be connected(because I2C protocol doesn't working)
	//init_dotmatrix();
//	set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor());
//	display_lcd_floor_info();
	//display_fnd_floor_info(curr_floor);
}

void set_curr_floor(uint8_t floor){
	curr_floor = floor;
}
uint8_t get_curr_floor(void){
	return curr_floor;
}

void set_target_floor(uint8_t floor){
	target_floor = floor;
}

uint8_t get_target_floor(void){
	return target_floor;
}

void set_curr_eleva_state(uint8_t eleva_state){
	curr_eleva_state = eleva_state;
}
void set_prev_eleva_state(uint8_t eleva_state){
	prev_eleva_state = eleva_state;
}
uint8_t get_curr_eleva_state(void){
	return curr_eleva_state;
}

void display_lcd_floor_info(void){
	char lcd_buff1[20],lcd_buff2[20];
	// clear
	lcd_command(CLEAR_DISPLAY);

	sprintf(lcd_buff1,"  Current Floor:%d", curr_floor);
	move_cursor(0, 0);
	lcd_string(lcd_buff1);

	sprintf(lcd_buff2,"Target Floor:%d", target_floor);
	move_cursor(1, 0);
	lcd_string(lcd_buff2);
}

void display_lcd_alert_info(void){
	char lcd_buff1[20];
	// clear
	lcd_command(CLEAR_DISPLAY);

	sprintf(lcd_buff1, "     Wrong Input!!! : %d", target_floor);
	move_cursor(0, 0);
	lcd_string(lcd_buff1);
}

uint8_t array_to_weighted_binary_int(uint8_t arr[], int size) {
	uint8_t result = 0;
	for (int i = 1; i < size; i++) {  // Start from index 1 to skip floor 0
		result += arr[i] * (1 << (i - 1));  // Weight is 2^(index-1)
	}
	return result;
}

uint8_t isFloorEmpty(uint8_t* pArr, int size) {
	int cnt = 0;
	for (int i = 1; i < size; i++) {
		if (pArr[i] == 0) {
			cnt++;
		}
	}
	return (cnt == size - 1) ? 1 : 0;
}

void choose_eleva_floor2(void){
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS) {
		if(target_floor_arr[1] == 0){
			target_floor_arr[1] = 1;
		}else if(target_floor_arr[1] == 1){
			target_floor_arr[1] = 0;
		}
	}else if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1)== BUTTON_PRESS) {
		if(target_floor_arr[2] == 0){
			target_floor_arr[2] = 1;
		}else if(target_floor_arr[2] == 1){
			target_floor_arr[2] = 0;
		}
	}else if(get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON2)== BUTTON_PRESS){
		if(target_floor_arr[3] == 0){
			target_floor_arr[3] = 1;
		}else if(target_floor_arr[3] == 1){
			target_floor_arr[3] = 0;
		}
	}else if(get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, BUTTON3)== BUTTON_PRESS){
		if(target_floor_arr[4] == 0){
			target_floor_arr[4] = 1;
		}else if(target_floor_arr[4] == 1){
			target_floor_arr[4] = 0;
		}
	}
}

void move_direct_check() {
	int temp[SIZE] = {0};
	uint8_t up = 0, down = 0;
//	printf("s,up : %d,down : %d\n",up,down);

	if (target_floor_arr[1] !=0 || target_floor_arr[2]!=0
			|| target_floor_arr[3]!=0 || target_floor_arr[4]!=0) {
		move_flag = 1;
	}

	if (move_flag == 1) {
		if (target_floor_arr[1] == 0 && target_floor_arr[2] == 0
			&& target_floor_arr[3] == 0 && target_floor_arr[4] == 0) {
				if(!(HAL_GPIO_ReadPin(PhotoSensor_EXTI0_GPIO_Port, PhotoSensor_EXTI0_Pin)||
						HAL_GPIO_ReadPin(PhotoSensor_EXTI1_GPIO_Port, PhotoSensor_EXTI1_Pin)||
						HAL_GPIO_ReadPin(PhotoSensor_EXTI4_GPIO_Port, PhotoSensor_EXTI4_Pin)||
						HAL_GPIO_ReadPin(PhotoSensor_EXTI5_GPIO_Port, PhotoSensor_EXTI5_Pin)))
				{
					return;
				}
				curr_eleva_state = ELEVA_STOP;
				move_flag=0;
		} else {
			switch (curr_eleva_state) {
			case ELEVA_STOP:
				if (target_floor_arr[1]) {
					temp[1] = 1 - current_floor;
					(temp[1] > 0) ? (up++) : ((temp[1] == 0) ?
							(curr_eleva_state =	ELEVA_STOP, target_floor_arr[1] = 0) :(down++));
				}
				if (target_floor_arr[2]) {
					temp[2] = 2 - current_floor;
					(temp[2] > 0) ? (up++) : ((temp[2] == 0) ?
							(curr_eleva_state =	ELEVA_STOP, target_floor_arr[2] = 0) :(down++));
				}
				if (target_floor_arr[3]) {
					temp[3] = 3 - current_floor;
					(temp[3] > 0) ? (up++) : ((temp[3] == 0) ?
							(curr_eleva_state =	ELEVA_STOP, target_floor_arr[3] = 0) :(down++));
				}
				if (target_floor_arr[4]) {
					temp[4] = 4 - current_floor;
					(temp[4] > 0) ? (up++) : ((temp[4] == 0) ?
							(curr_eleva_state =	ELEVA_STOP, target_floor_arr[4] = 0) :(down++));
				}
				if (up > down) {
					curr_eleva_state = ELEVA_START_BOTTOM_UP;
				} else if (down > up) {
					curr_eleva_state = ELEVA_START_TOP_DOWN;
				} else {
					// down = up
					// 계속 방향을 유지하게 해줘야한다.
					if (prev_eleva_state == ELEVA_START_BOTTOM_UP) {
						curr_eleva_state = ELEVA_START_BOTTOM_UP;
					}
					if (prev_eleva_state == ELEVA_START_TOP_DOWN) {
						curr_eleva_state = ELEVA_START_TOP_DOWN;
					}
				}
				break;
			case ELEVA_START_BOTTOM_UP:
				if (curr_floor == 4)
					curr_eleva_state = ELEVA_STOP;
				break;
			case ELEVA_START_TOP_DOWN:
				if (curr_floor == 1)
					curr_eleva_state = ELEVA_STOP;
				break;
			}
		}// end else
	} // end move_flag
}



