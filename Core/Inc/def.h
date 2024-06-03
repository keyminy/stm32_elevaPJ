#ifndef INC_DEF_H_
#define INC_DEF_H_

typedef enum{
	MOTOR_IDLE
	,MOTOR_FORWARD
	,MOTOR_BACKWARD
} MOTOR_STATE;

typedef enum{
	ELEVA_STOP = 0,
	ELEVA_START_BOTTOM_UP,
	ELEVA_START_TOP_DOWN
} ELEVATOR_STATE;

typedef enum{
	FLOOR_1 = 1,
	FLOOR_2,
	FLOOR_3,
	FLOOR_4
} Floor;

#endif /* INC_DEF_H_ */
