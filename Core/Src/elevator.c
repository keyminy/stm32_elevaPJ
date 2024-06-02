#include "elevator.h"
#include "def.h"
#include "dotmatrix.h"
#include "i2c_lcd.h"

static uint8_t curr_floor;
static uint8_t target_floor;
static uint8_t curr_eleva_state;

void elevator_init(void){
	/*initialize elevator state*/
	curr_floor = FLOOR_1; // In the initial state, the elevator is on the first floor.
	target_floor = FLOOR_1;
	curr_eleva_state = ELEVA_STOP;
	/*peripheral*/
	i2c_lcd_init(); // warning LCD should be connected(because I2C protocol doesn't working)
	//init_dotmatrix();

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

	sprintf(lcd_buff1, "Wrong Input!!! : %d", target_floor);
	move_cursor(0, 0);
	lcd_string(lcd_buff1);
}







