#include "dotmatrix.h"
#include "elevator.h"
#include "def.h"

extern SPI_HandleTypeDef hspi2;
extern volatile int TIM2_1ms_DOT_counter;

uint8_t col[4]={0,0,0,0};


static const uint8_t floor_num[5][8] =
{
    {
    0b00011000,
    0b00111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
    0b00011000
    }, // 0
	{
	0b00000000,
	0b00011000,
	0b00111000,
	0b01111000,
	0b00011000,
	0b00011000,
	0b01111110,
	0b00000000
	}, // 1
	{
	0b00000000,
	0b00111100,
	0b01100110,
	0b01000110,
	0b00001100,
	0b00011000,
	0b01111110,
	0b00000000
	}, // 2
	{
	0b00000000,
	0b00111100,
	0b01000010,
	0b00000010,
	0b00001110,
	0b01000010,
	0b00111100,
	0b00000000
	}, // 3
	{
	0b00000000,
	0b00001100,
	0b00010100,
	0b00100100,
	0b01111110,
	0b00000100,
	0b00000100,
	0b00000000
	}  // 4
};

static const uint8_t up_arrow[] =
{
	0b00011000,
	0b00111100,
	0b01111110,
	0b11111111,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00000000
};

static const uint8_t down_arrow[] =
{
	0b00000000,
	0b00111100,
	0b00111100,
	0b00111100,
	0b11111111,
	0b01111110,
	0b00111100,
	0b00011000
};

static const uint8_t all_off[] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

unsigned char display_data[8];  // Final 8x8 Output Data
unsigned char scroll_buffer[30][8] = {0};  // Buffer containing data to be scrolled <- blank,arrow,number
int number_of_character = 4;  // Number of characters to output

void clear_dotmatrix_buffer(void){
	for	(int i = 0; i < 30; i++) // 1부터 시작
	{
		for (int j = 0; j < 8; j++) // scroll_buffer[0] = blank
		{
			scroll_buffer[i][j] = all_off[j];
		}
	}
}
void control_dotmatrix(uint8_t curr_eleva_state){
	static int count=0;  // 컬럼 count
	static int index=0;  // scroll_buffer의 2차원 index값
	static uint32_t past_time=0;  // 이전 tick값 저장
	uint32_t now = HAL_GetTick();  // 1ms
	static uint8_t i_iter =0;
	//(반장님조언)- 타이머변수를 이용해서 switch가기전(SPI하기전) 1ms마다 호출되게
	switch(curr_eleva_state){
	case ELEVA_STOP:
		set_dotmatrix_buffer(get_curr_eleva_state(),get_curr_floor());
        if(TIM2_1ms_DOT_counter >= 1){
        	TIM2_1ms_DOT_counter = 0;
        	// for문 행 8행 돈다
			col[0] = ~(1 << i_iter); // 0000_0001 -->(one's complement 1111_1110)
			col[1] = display_data[i_iter];
			HAL_SPI_Transmit(&hspi2, col, 2, 1); // 2byte전송 timeout은 1ms
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			//HAL_Delay(1); // 잔상효과 없애기위해 1ms
			i_iter++;
			i_iter%=8;
        }
		break;
	case ELEVA_START_BOTTOM_UP:
		//now = HAL_GetTick();  // 1ms
		// 1ms인 한 틱을 읽는다.
		// 틱값을 읽으면서 현재 몇 ms가 됬는지 안됬는지 check함
		// 500ms마다 한 칸 씩 shift하면서 돌리겠다.
		// 1.처음시작시 past_time=0; now: 500 --> past_time=500
		if (now - past_time >= 500) // scroll every 500ms
		{
			past_time = now;
			for (int i = 0; i < 8; i++) {
				// upward scrolling
				 display_data[i] = (scroll_buffer[index][(i + count) % 8]);
			}
			if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
			{
				count = 0;
				index++;  // 다음 scroll_buffer로 이동
				if (index == number_of_character + 1)
					index = 0;
				// 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
			}
		}
		if(TIM2_1ms_DOT_counter >= 1){
			TIM2_1ms_DOT_counter = 0;
			// common annode type
			// column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
			// for문 행 8행 돈다
			col[0] = ~(1 << i_iter); // 0000_0001 -->(one's complement 1111_1110)
			col[1] = display_data[i_iter];
			HAL_SPI_Transmit(&hspi2, col, 2, 1); // 2byte전송 timeout은 1ms
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			//HAL_Delay(1); // 잔상효과 없애기위해 1ms
			i_iter++;
			i_iter%=8;
		}
		break;
	case ELEVA_START_TOP_DOWN:
		// 1ms인 한 틱을 읽는다.
		// now = HAL_GetTick();  // 1ms
		// 틱값을 읽으면서 현재 몇 ms가 됬는지 안됬는지 check함
		// 500ms마다 한 칸 씩 shift하면서 돌리겠다.
		// 1.처음시작시 past_time=0; now: 500 --> past_time=500
		if (now - past_time >= 500) // 500ms scroll
		{
			past_time = now;
			for (int i = 0; i < 8; i++) {
				// downward scrolling
				display_data[i] = (scroll_buffer[index][(i + 8 - count) % 8]);
			}
			if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
			{
				count = 0;
				index++;  // 다음 scroll_buffer로 이동
				if (index == number_of_character + 1)
					index = 0;
				// 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
			}
		}
        if(TIM2_1ms_DOT_counter >= 1){
        	TIM2_1ms_DOT_counter = 0;
			// common annode type
			// column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
			// for문 행 8행 돈다
			col[0] = ~(1 << i_iter); // 0000_0001 -->(one's complement 1111_1110)
			col[1] = display_data[i_iter];
			HAL_SPI_Transmit(&hspi2, col, 2, 1); // 2byte전송 timeout은 1ms
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			//HAL_Delay(1); // 잔상효과 없애기위해 1ms
			i_iter++;
			i_iter%=8;
		}
		break;
	}
}

void set_dotmatrix_buffer(uint8_t curr_eleva_state, uint8_t floor)
{
	clear_dotmatrix_buffer();

	switch (curr_eleva_state) {
		case ELEVA_STOP:
			for(int i=0; i <8; i++){
				display_data[i] = floor_num[floor][i];
			}
			// No need to scroll in the stop status
//			for(int i=1; i < 8; i++){
//				for(int j=0; j <8;j++){
//					// scroll_buffer[0] = blank
//					scroll_buffer[i][j] = floor_num[floor][j];
//				}
//			}
			break;
		case ELEVA_START_BOTTOM_UP: //display ↑, next floor number
			for(int i=0; i <8; i++){
				display_data[i] = up_arrow[i];
			}
			for(int i=1; i < 8; i++){
				for(int j=0; j <8;j++){
					// scroll_buffer[0] = blank
					scroll_buffer[i][j] = (i == 1) ? up_arrow[j] : floor_num[floor][j];
				}
			}

			break;
		case ELEVA_START_TOP_DOWN: //display ↓, previous floor number
			for(int i=0; i < 8; i++){
				display_data[i] = down_arrow[i];
			}
			for(int i=1; i < 8; i++){
				for(int j=0; j <8;j++){
					// scroll_buffer[0] = blank
					 scroll_buffer[i][j] = (i == 1) ? down_arrow[j] : floor_num[floor][j];
				}
			}
			break;
	}
}

#if 0
/* testing */
// 초기화 작업
// 1. display_data에 number_data[0]에 있는 내용 복사
// 2. number_data를 scroll_buffer에 복사
// 3. dotmatrix의 led를 off
//void init_dotmatrix(void)
//{
//	for (int i=0; i < 8; i++)
//	{
//		display_data[i] = number_data[i][0];
//	}
//	for (int i=1; i < number_of_character+1; i++)
//	{
//		for (int j=0; j < 8; j++) // scroll_buffer[0] = blank
//		{
//			scroll_buffer[i][j] = number_data[i-1][j];
//			// i 는 1~11번(10번), j는 0~7번(8번 반복)
//		}
//	}
//
//}
// scroll 문자 출력 프로그램
int dotmatrix_main(void)
{
	static int count=0;  // 컬럼 count
	static int index=0;  // scroll_buffer의 2차원 index값
	static uint32_t past_time=0;  // 이전 tick값 저장

	init_dotmatrix();

	while(1)
	{
		// 1ms인 한 틱을 읽는다.
		uint32_t now = HAL_GetTick();  // 1ms
		// pc_command_processing(); // UART input comes in (word parameter corresponding to parameter)
		// 틱값을 읽으면서 현재 몇 ms가 됬는지 안됬는지 check함
		// 500ms마다 한 칸 씩 shift하면서 돌리겠다.
		// 1.처음시작시 past_time=0; now: 500 --> past_time=500
		if (now - past_time >= 500) // 500ms scroll
		{
			past_time = now;
			for (int i=0; i < 8; i++)
			{
				display_data[i] = (scroll_buffer[index][i] >> count) |
						(scroll_buffer[index+1][i] << 8 - count);
			}
			if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
			{
				count =0;
				index++;  // 다음 scroll_buffer로 이동
				if (index == number_of_character+1) index=0;
				// 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
			}
		}
		//display_data를 여기서 찍어보기
//		for(int i=0; i< 8;i++){
//			printf("%u\n",display_data[i]);
//		}
		for (int i=0; i < 8; i++)
		{
			// 공통 양극 방식
			// column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
			// for문 행 8행 돈다
			col[0] = ~(1 << i); // 0000_0001 -->(one's complement 1111_1110)
			col[1] = display_data[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1); // 2byte전송 timeout은 1ms
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1); // 잔상효과 없애기위해 1ms
		}
	}
	return 0;
}


void dotmatrix_main_test()
{
  while (1)
  {
        for (int i=0; i < 8; i++)
        {
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = down_arrow[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
        }
  }
}

#endif

