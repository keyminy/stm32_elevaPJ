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
volatile uint8_t curr_floor_arr[SIZE]={0,};
volatile uint8_t current_floor=0;
volatile char move_flag = 0;
volatile char open_flag = 0;
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

//	HAL_GPIO_TogglePin(GPIOB, built_LED_Pin);
//	HAL_Delay(200);

	choose_eleva_floor2();

	if (!isFloorEmpty(target_floor_arr,5)) {
		move_flag = 1;
	}
	if (move_flag == 1) {
		if (isFloorEmpty(target_floor_arr,5)) {
			// 이런 경우는 생길 수 없을거 같은데
			// 이동 중 취소한 상태, 가장 가까운 층에 멈출 수 있게하기
			set_curr_eleva_state(ELEVA_STOP);
		} else {
			switch (curr_eleva_state) {
			case ELEVA_STOP:
				if(target_floor_arr[1]){
					temp[1] = 1 - current_floor;
					printf("go %d\n",temp[1]); // 254.. it should be 1-3=-2
					printf("? %d\n",current_floor); // 3
					(temp[1]>0)?(up++):((temp[1]==0)?(curr_eleva_state=ELEVA_STOP):(down++));
				}
				if(target_floor_arr[2]){
					temp[2] = 2 - current_floor;
					(temp[2]>0)?(up++):((temp[2]==0)?(curr_eleva_state=ELEVA_STOP):(down++));
				}
				if(target_floor_arr[3]){
					temp[3] = 3 - current_floor;
					(temp[3]>0)?(up++):((temp[3]==0)?(curr_eleva_state=ELEVA_STOP):(down++));
				}
				if(target_floor_arr[4]){
					temp[4] = 4 - current_floor;
					(temp[4]>0)?(up++):((temp[4]==0)?(curr_eleva_state=ELEVA_STOP):(down++));
				}
				if(up>down){
					curr_eleva_state = ELEVA_START_BOTTOM_UP;
				}else if(down>up){
					curr_eleva_state = ELEVA_START_TOP_DOWN;
				}else{
					// down = up
				}
				printf("up:%d,down%d\n",up,down);

				break;
			case ELEVA_START_BOTTOM_UP:
//				if(target_floor_arr[current_floor] == 1){
//					target_floor_arr[current_floor] = 0;
//					curr_eleva_state = ELEVA_STOP;
//				}
//				if (curr_floor > target_floor) {
//					// TODO : Check if a higher floor is reserved in the floor_queue, go to the higher floor, and then go to the lower reserved floor.
//
//				} else if (current_floor < purpose_floor) {
//					// If there are floors to go up, keep moving up.
//					;
//				}
				break;
			case ELEVA_START_TOP_DOWN:
//				if(target_floor_arr[current_floor] == 1){
//					target_floor_arr[current_floor] = 0;
//					curr_eleva_state = ELEVA_STOP;
//				}
//				if (curr_floor > target_floor) {
//					// If there are floors to go down, keep moving down.
//					;
//				} else if (curr_floor < target_floor) {
//					// TODO : Check if a lower floor is reserved in the floor_queue, go to the lower floor, and then go to the higher reserved floor.
//				}
				break;
			}
		}
		move_flag = 0;
	}
}



