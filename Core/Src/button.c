/*
 * button.c
 *
 * Created: 2024-04-25 오후 1:30:31
 *  Author: kccistc
 */ 
#include "button.h" 
#include "def.h"
#include "elevator.h"
#include "dotmatrix.h"

extern int motor_state; // default : MOTOR_IDLE

unsigned char button_status[BUTTON_NUMBER] =
	{BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE};
	

char button0_count=0;

void button0_check(void)
{
	// 버튼을 한번 울렀다 떼었는가 ?
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0)== BUTTON_PRESS)
	{
		button0_count++;
		button0_count %= 4;
	}
}


// 버튼이 온전하게 눌렸다 떼어진 상태 이면 1을 리턴 한다. 
int get_button(GPIO_TypeDef  *GPIO, int button_pin, int button_num)  // 예) GPIOC BUTTON0 4
{         
	int current_state; 

	current_state = HAL_GPIO_ReadPin(GPIO, button_pin);  //  active low press:0 , Release:1 버튼값을 읽는다.
	if (current_state == BUTTON_PRESS && button_status[button_num]==BUTTON_RELEASE)  // 버튼이 처음 눌려진상태
	{
		HAL_Delay(60);   // 노이즈가 지나가기를 기다린다
		button_status[button_num]=BUTTON_PRESS;  // 처음 눌려진 상태가 아니다
		return BUTTON_RELEASE;        // 아직은 완전히 눌렸다 떼어진 상태가 아니다.
	}     
	else if (button_status[button_num]==BUTTON_PRESS && current_state==BUTTON_RELEASE)
	       // 버튼이 이전에 눌러진 상태였으며 현재는 떼어진 상태
	{
		button_status[button_num]=BUTTON_RELEASE;   // 다음 버튼 상태를 체크하기위해 초기화
		HAL_Delay(60);   // 노이즈가 지나가기를 기다린다
	    return BUTTON_PRESS;    // 완전히 눌렸다 떼어진 상태임을 리턴
	}
	 
	 return BUTTON_RELEASE;   // 아직 완전히 스위치를 눌렀다 뗀 상태가 아닌경우 나
	             // 스위치가 open 된 상태                   	
}

// Choose the floor of elevator based on the state of the button being pressed.
void choose_eleva_floor(void){
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0)== BUTTON_PRESS){
	  //button 0 : Elavator FLOOR_1
		if(get_curr_floor() == FLOOR_1){
			set_curr_eleva_state(ELEVA_STOP);
			display_lcd_alert_info();
			HAL_Delay(2000);
		}else if(get_curr_floor() > FLOOR_1){
			// when get_curr_floor() is FLOOR_2 or FLOOR_3
			set_curr_eleva_state(ELEVA_START_TOP_DOWN);
		}
		  set_target_floor(FLOOR_1);
		  display_lcd_floor_info();
	  //display_lcd_floor_info();
	}else if(get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1)== BUTTON_PRESS){
	  //button 1 : Elavator FLOOR_2
		if(get_curr_floor() == FLOOR_2){
			set_curr_eleva_state(ELEVA_STOP);
			display_lcd_alert_info();
			HAL_Delay(2000);
		}else if(get_curr_floor() > FLOOR_2){
			// when get_curr_floor() is FLOOR_3
			set_curr_eleva_state(ELEVA_START_TOP_DOWN);
			//set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor()-1);
		}else if(get_curr_floor() < FLOOR_2){
			// when get_curr_floor() is FLOOR_1
			set_curr_eleva_state(ELEVA_START_BOTTOM_UP);
			//set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor()+1);
		}
	  set_target_floor(FLOOR_2);
	  display_lcd_floor_info();
	}else if(get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON2)== BUTTON_PRESS){
	  //button2 : Elavator FLOOR_3
		if (get_curr_floor() == FLOOR_3) {
			set_curr_eleva_state(ELEVA_STOP);
			display_lcd_alert_info();
			HAL_Delay(2000);
		} else if (get_curr_floor() < FLOOR_3) {
			// when get_curr_floor() is FLOOR_1 or FLOOR_2
			set_curr_eleva_state(ELEVA_START_BOTTOM_UP);
			//set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor()+1);
		}
	  set_target_floor(FLOOR_3);
	  display_lcd_floor_info();
	}
}
