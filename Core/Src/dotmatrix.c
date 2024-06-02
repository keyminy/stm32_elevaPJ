#include "dotmatrix.h"
#include "elevator.h"
#include "def.h"

#define SCROLL_UP 1
#define SCROLL_DOWN 2

extern SPI_HandleTypeDef hspi2;

uint8_t col[4]={0,0,0,0};


static const uint8_t floor_num[10][8] =
{
	{
		0b00000000, // 0
		0b00000000,
		0b01111110,
		0b10000001,
		0b10000001,
		0b01111110,
		0b00000000,
		0b00000000
	},
	{
		0b00000000, // 1
		0b00000000,
		0b00000001,
		0b11111111,
		0b01000001,
		0b00100001,
		0b00000000,
		0b00000000
	},
	{
		0b00000000, // 2
		0b00000001,
		0b01110001,
		0b10001001,
		0b10000101,
		0b01000011,
		0b00100001,
		0b00000000
	},
	{
		0b00000000, // 3
		0b00000000,
		0b01100110,
		0b10011001,
		0b10010001,
		0b10010001,
		0b00000000,
		0b00000000
	},
	{
		0b00000000, // 4
		0b00000100,
		0b11111111,
		0b01000100,
		0b00100100,
		0b00010100,
		0b00001100,
		0b00000000
	}
};

static const uint8_t up_arrow[] =
{
	0b00001110,
	0b00011100,
	0b00111000,
	0b01110000,
	0b01110000,
	0b00111000,
	0b00011100,
	0b00001110
};

static const uint8_t down_arrow[] =
{
	0b01110000,
	0b00111000,
	0b00011100,
	0b00001110,
	0b00001110,
	0b00011100,
	0b00111000,
	0b01110000
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

unsigned char display_data[8];  // 최종 8x8 출력할 데이터
unsigned char scroll_buffer[50][8] = {0};  // 스코롤할 데이타가 들어있는 버퍼
int number_of_character = 11;  // 출력할 문자 갯수


void clear_dotmatrix_buffer(void){
	for	(int buffer_idx = 0; buffer_idx < 50; buffer_idx++) // 1부터 시작
	{
		for (int matrix_idx = 0; matrix_idx < 8; matrix_idx++) // scroll_buffer[0] = blank
		{
			scroll_buffer[buffer_idx][matrix_idx] = all_off[matrix_idx];
		}
	}
}
void control_dotmatrix(uint8_t curr_eleva_state){
	static int count=0;  // 컬럼 count
	static int index=0;  // scroll_buffer의 2차원 index값
	static uint32_t past_time=0;  // 이전 tick값 저장
	uint32_t now = HAL_GetTick();  // 1ms

	switch(curr_eleva_state){
	case ELEVA_STOP:
		count = 0;
		index = 0;
		past_time = 0;
		clear_dotmatrix_buffer();
		break;
	case ELEVA_START_BOTTOM_UP:
		now = HAL_GetTick();  // 1ms
		// 1ms인 한 틱을 읽는다.
		// 틱값을 읽으면서 현재 몇 ms가 됬는지 안됬는지 check함
		// 500ms마다 한 칸 씩 shift하면서 돌리겠다.
		// 1.처음시작시 past_time=0; now: 500 --> past_time=500
		if (now - past_time >= 500) // 500ms scroll
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
		for (int i=0; i < 8; i++)
		{
			// common annode type
			// column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
			// for문 행 8행 돈다
			col[0] = ~(1 << i); // 0000_0001 -->(one's complement 1111_1110)
			col[1] = display_data[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1); // 2byte전송 timeout은 1ms
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1); // 잔상효과 없애기위해 1ms
		}
		break;
	case ELEVA_START_TOP_DOWN:
		// 1ms인 한 틱을 읽는다.
		now = HAL_GetTick();  // 1ms
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
		for (int i=0; i < 8; i++)
		{
			// common annode type
			// column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
			// for문 행 8행 돈다
			col[0] = ~(1 << i); // 0000_0001 -->(one's complement 1111_1110)
			col[1] = display_data[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1); // 2byte전송 timeout은 1ms
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1); // 잔상효과 없애기위해 1ms
		}
		break;
	}
}

void set_dotmatrix_buffer(uint8_t curr_eleva_state, uint8_t target_floor)
{
	clear_dotmatrix_buffer();

	switch (curr_eleva_state) {
		case ELEVA_STOP:
			break;
		case ELEVA_START_BOTTOM_UP:
			for	(int buffer_idx = 0; buffer_idx < 50; buffer_idx++)
			{
				for (int matrix_idx = 0; matrix_idx < 8; matrix_idx++) // scroll_buffer[0] = blank
				{
					if (buffer_idx == 1) { scroll_buffer[buffer_idx][matrix_idx] = floor_num[target_floor][matrix_idx]; }
					else if (buffer_idx == 0) { scroll_buffer[buffer_idx][matrix_idx] = down_arrow[matrix_idx]; }
				}
			}
			break;
		case ELEVA_START_TOP_DOWN:
			for	(int buffer_idx = 0; buffer_idx < 50; buffer_idx++)
			{
				for (int matrix_idx = 0; matrix_idx < 8; matrix_idx++) // scroll_buffer[0] = blank
				{
					if (buffer_idx == 0) { scroll_buffer[buffer_idx][matrix_idx] = up_arrow[matrix_idx]; }
					else if (buffer_idx == 1) { scroll_buffer[buffer_idx][matrix_idx] = floor_num[target_floor][matrix_idx]; }
				}
			}
			break;
	}
}


void dotmatrix_main_test()
{
  dotmatrix_main();

  while (1)
  {
        for (int i=0; i < 8; i++)
        {
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = all_off[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);
			GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
			HAL_Delay(1);
        }
  }
}

