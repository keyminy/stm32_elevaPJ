#include "elevator.h"
#include "def.h"
#include "dotmatrix.h"
#include "button.h"
#include "i2c_lcd.h"
#include "queue.h"

volatile static uint8_t curr_floor;
volatile static uint8_t target_floor;
volatile static uint8_t curr_eleva_state = ELEVA_STOP;

/* ref */
volatile Queue floor_queue;
volatile uint8_t target_floor_arr={0};
volatile uint8_t curr_floor_arr={0};
volatile char move_flag = 0;
volatile char open_flag = 0;
/* end ref */

void elevator_init(void){
	/*initialize elevator state*/
	curr_floor = FLOOR_1; // In the initial state, the elevator is on the first floor.
//	target_floor = FLOOR_1;
	createQueue(&floor_queue);

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



void move_direct_check() {
	if (!isQueueEmpty(&floor_queue)) {
		target_floor = peek(&floor_queue);
		move_flag = 1;
	}
	if (move_flag == 1) {
		if (target_floor == 0) {
			// 이런 경우는 생길 수 없을거 같은데
			// initial state...
			// The elevator should stop.
			set_curr_eleva_state(ELEVA_STOP);
		} else {
			switch (curr_eleva_state) {
			case ELEVA_STOP:
				if (curr_floor == target_floor) {
					remove_floor(&floor_queue,target_floor);
					//open_flag = 1; // open door
				} else if (curr_floor > target_floor) {
					curr_eleva_state = ELEVA_START_TOP_DOWN;
				} else if (curr_floor < target_floor) {
					curr_eleva_state = ELEVA_START_BOTTOM_UP;
				}
				break;
			case ELEVA_START_BOTTOM_UP:
				if (curr_floor > target_floor) {
					// TODO : Check if a higher floor is reserved in the floor_queue, go to the higher floor, and then go to the lower reserved floor.

				} else if (current_floor < purpose_floor) {
					// If there are floors to go up, keep moving up.
					;
				}
				break;
			case ELEVA_START_TOP_DOWN:
				if (curr_floor > target_floor) {
					// If there are floors to go down, keep moving down.
					;
				} else if (curr_floor < target_floor) {
					// TODO : Check if a lower floor is reserved in the floor_queue, go to the lower floor, and then go to the higher reserved floor.
				}
				break;
			}
		}
		move_flag = 0;
	}
}



