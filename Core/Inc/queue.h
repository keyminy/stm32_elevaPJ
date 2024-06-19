/*
 * queue.h
 *
 *  Created on: 2024. 6. 17.
 *      Author: HARMAN-27
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include "main.h"

#define QUEUE_SIZE 10

typedef struct _queue {
	uint8_t floors[QUEUE_SIZE];
	int front;
	int rear;
	int size;
} Queue;

void createQueue(Queue* qp);
// isQueueEmpty : front==rear
int isQueueEmpty(const Queue* qp);

// isQueueFull : When the rear pointer points to the buffer zone.
int isQueueFull(const Queue* qp);
void enqueue(Queue* qp, uint8_t floor);
uint8_t dequeue(Queue* qp);
void printQueue(const Queue* qp);
uint8_t is_floor_existed(Queue* qp, uint8_t floor);
void remove_floor(Queue* qp, uint8_t floor);
void toggle_floor_request(Queue* qp, uint8_t floor);
uint8_t peek(Queue* qp);

#endif /* INC_QUEUE_H_ */
