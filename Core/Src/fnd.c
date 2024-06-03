#include "fnd.h"
#include "elevator.h"

extern volatile int TIM2_1ms_FND_counter;

/* note : common anode fnd!!*/
void init_fnd(){
    HAL_GPIO_WritePin(FND_DIGIT1_GPIO_Port, FND_DIGIT1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FND_DIGIT2_GPIO_Port, FND_DIGIT2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FND_DIGIT3_GPIO_Port, FND_DIGIT3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FND_DIGIT4_GPIO_Port, FND_DIGIT4_Pin, GPIO_PIN_RESET);
}

void fnd_shift_clock(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}

void fnd_latch_clock(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
}

void fnd_send_data(uint8_t data) {
	for (int i = 0; i < 8; i++) {
		// Send the MSB first
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,
				(data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		fnd_shift_clock();
		data <<= 1;
	}
	fnd_latch_clock();
}

void fnd_display(){
    unsigned char fnd_font[] = {
        0xC0, // 0
        0xF9, // 1
        0xA4, // 2
        0xB0, // 3
        0x99, // 4
        0x92, // 5
        0x82, // 6
        0xF8, // 7
        0x80, // 8
        0x90  // 9
    };

    static int digit_position = 0;  // Position of the current digit being displayed (0-3)

    // Display the current floor of the elevator every 4ms
    if(TIM2_1ms_FND_counter >= 4){
        TIM2_1ms_FND_counter = 0;

        // Turn off all digits
        HAL_GPIO_WritePin(FND_DIGIT1_GPIO_Port, FND_DIGIT1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FND_DIGIT2_GPIO_Port, FND_DIGIT2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FND_DIGIT3_GPIO_Port, FND_DIGIT3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FND_DIGIT4_GPIO_Port, FND_DIGIT4_Pin, GPIO_PIN_RESET);

        // Send segment data to the shift register
       fnd_send_data(fnd_font[get_curr_floor()]); // <- I want to display number "1"

       HAL_GPIO_WritePin(FND_DIGIT1_GPIO_Port, FND_DIGIT1_Pin, GPIO_PIN_SET); // Turn on digit1

    }
}
