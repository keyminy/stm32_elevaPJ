#ifndef INC_DEF_H_
#define INC_DEF_H_
#include "main.h"

typedef enum{
	MOTOR_IDLE
	,MOTOR_FORWARD
	,MOTOR_BACKWARD
} MOTOR_STATE;

typedef enum{
	SERVO_OPEN // 0 dgree
	,SERVO_LOCK // 180 dgree
} SERVO_STATE;

typedef enum{
	ELEVA_STOP = 0,
	ELEVA_START_BOTTOM_UP,
	ELEVA_START_TOP_DOWN
} ELEVATOR_STATE;

typedef enum{
	FLOOR_1 = 1,
	FLOOR_2 = 2,
	FLOOR_3 = 3,
	FLOOR_4 = 4
} Floor;

#define f1 0b00000011
#define f2 0b00001100
#define f3 0b00110000
#define f4 0b11000000

#define f1_PORT GPIOA
#define f2_PORT GPIOA
#define f3_PORT GPIOA
#define f4_PORT GPIOA

#define f1_PIN GPIO_PIN_0
#define f2_PIN GPIO_PIN_1
#define f3_PIN GPIO_PIN_4
#define f4_PIN GPIO_PIN_5

#endif /* INC_DEF_H_ */
