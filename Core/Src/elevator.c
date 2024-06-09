#include "elevator.h"
#include "def.h"
#include "dotmatrix.h"
#include "button.h"
#include "i2c_lcd.h"

volatile static uint8_t curr_floor = 0;
volatile static uint8_t target_floor = 0;
volatile static uint8_t curr_eleva_state = ELEVA_STOP;
volatile char move_flag = 0;
volatile char open_flag = 0;


void elevator_init(void){
	/*initialize elevator state*/
//	curr_floor = FLOOR_1; // In the initial state, the elevator is on the first floor.
//	target_floor = FLOOR_1;
	curr_floor = 0; // In the initial state, the elevator is on the first floor.
	target_floor = 0;
	curr_eleva_state = ELEVA_STOP;
	/*peripheral*/
	i2c_lcd_init(); // warning LCD should be connected(because I2C protocol doesn't working)
	//init_dotmatrix();
	set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor());
	display_lcd_floor_info();
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


void eleva_button_check(void){
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS) {
		if((target_floor & f1) == f1){
			target_floor &= ~f1;
		}else{
			target_floor |= f1;
		}
	}else if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1)== BUTTON_PRESS) {
		if((target_floor & f2) == f2){
			target_floor &= ~f2;
		}else{
			target_floor |= f2;
		}
	}else if(get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON2)== BUTTON_PRESS){
		if((target_floor & f3) == f3){
			target_floor &= ~f3;
		}else{
			target_floor |= f3;
		}
	}else if(get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, BUTTON3)== BUTTON_PRESS){
		if((target_floor & f4) == f4){
			target_floor &= ~f4;
		}else{
			target_floor |= f4;
		}
	}
}

void move_direct_check() {
	static uint8_t old_TF = 0; // 함수내에 static선언하면,해당 함수내에서만 visible하다.
	if (old_TF != target_floor) {
		move_flag = 1;
		old_TF = target_floor;
	}
	if (move_flag == 1) {
		if (target_floor == 0) {
			// stop상태
			// HAL_GPIO_ReadPin를 통해, 가장 가까운 층에서 멈추게 한다
			// HAL_GPIO_ReadPin = 1이 발생해야 curr_eleva_state = ELEVA_STOP;로 업데이트되어 정지됨.
			if (!(HAL_GPIO_ReadPin(floor_1_PIN, floor1_PIN)
				|| HAL_GPIO_ReadPin(floor_2_PIN, floor2_PIN)
				|| HAL_GPIO_ReadPin(floor_3_PIN, floor3_PIN)
				|| HAL_GPIO_ReadPin(floor_4_PIN, floor4_PIN))
				) {
				return;
			}
			curr_eleva_state = ELEVA_STOP;
		}
		else {
			switch (curr_eleva_state) {
			case ELEVA_STOP:
				if (curr_floor == target_floor) {
					target_floor &= ~curr_floor; // Clear the target floor.
					open_flag = 1; // 문열기
				}
				else if (curr_floor > target_floor) {
					curr_eleva_state = ELEVA_START_TOP_DOWN;
				}
				else if (curr_floor < target_floor) {
					curr_eleva_state = ELEVA_START_BOTTOM_UP;
				}
				break;
			case ELEVA_START_BOTTOM_UP:
				if (curr_floor > target_floor) {
					// curr : 0b00001100, target : 0b00000011
					// 밖에서 아랫층사람 눌렀는데, 올라가던 중 윗층 취소. 그럼 아래로 넘어가야지.
					curr_eleva_state = ELEVA_START_TOP_DOWN;
				}
				else if (curr_floor < target_floor) {
					// If there are floors to go up, keep moving up.
					;
				}
				break;
			case ELEVA_START_TOP_DOWN:
				if (curr_floor > target_floor) {
					// If there are floors to go down, keep moving down.
					;
				}
				else if (curr_floor < target_floor) {
					//밖에서 윗층사람 눌렀는데, 내려가던 중 아랫층 취소. 그럼 위로 바껴야지.
					uint8_t temp_cf = curr_floor;
					uint8_t temp_tf = target_floor;

					while (temp_cf != 0) {
						temp_cf = temp_cf << 2; // overflow to remove the high order two bits.
						temp_tf = temp_tf << 2;
					}
					if (temp_tf != 0) {
						// 계속 내려가야할 층이 남아있는 경우
					}
					else {
						curr_eleva_state = ELEVA_START_BOTTOM_UP;
					}

				break;
			}
		}
		move_flag = 0;
	}
}




