#include "queue.h"
#include "elevator.h"

void createQueue(Queue* qp) {
	qp->front = 0;
	qp->rear = 0;
	qp->size = 0;
}

// isQueueEmpty : front==rear
int isQueueEmpty(const Queue* qp) {
	return (qp->front == qp->rear) ? 1 : 0;
}

// isQueueFull : When the rear pointer points to the buffer zone.
int isQueueFull(const Queue* qp) {
	return (qp->front == (qp->rear + 1) % QUEUE_SIZE) ? 1 : 0;
}

void enqueue(Queue* qp, uint8_t floor) {
	if (isQueueFull(qp) == 1) {
		return;
	}
	qp->floors[qp->rear] = floor;
	qp->rear = (qp->rear + 1) % QUEUE_SIZE;
	qp->size++;
}

uint8_t dequeue(Queue* qp) {
	if (isQueueEmpty(qp) == 1) {
		return 0;
	}
	uint8_t floor = qp->floors[qp->front];
	qp->front = (qp->front + 1) % QUEUE_SIZE;
	qp->size--;
	return floor;
}

void printQueue(const Queue* qp) {
	if (isQueueEmpty(qp) == 1) {
		printf("queue is empty!!..");
		return;
	}
	for (int i = qp->front; i != qp->rear; i = (i + 1) % QUEUE_SIZE) {
		printf("%d ", qp->floors[i]);
	}
	printf("\n");
}

uint8_t is_floor_existed(Queue* qp,uint8_t floor) {
	if (!isQueueEmpty(qp)) {
		for (int i = qp->front; i != qp->rear; i = (i + 1) % QUEUE_SIZE) {
			if (qp->floors[(qp->front + i) % QUEUE_SIZE] == floor) {
				return 1;
			}
		}
	}
	return 0;
}

void remove_floor(Queue* qp, uint8_t floor) {
	if (isQueueEmpty(qp) == 1) {
		return;
	}
	int new_front = qp->front;
	int new_rear = qp->rear;
	int new_size = 0;
	uint8_t new_floors[QUEUE_SIZE];

	for (int i = qp->front; i != qp->rear; i = (i + 1) % QUEUE_SIZE) {
		uint8_t curr_temp_floor = qp->floors[(qp->front + i) % QUEUE_SIZE];
		if (curr_temp_floor != floor) {
			new_floors[new_size++] = curr_temp_floor;
		}
	}
	for (int i = 0; i < new_size; i++) {
		qp->floors[(new_front + i) % QUEUE_SIZE] = new_floors[i];
	}

	qp->front = new_front;
	qp->rear = (new_front + new_size)  % QUEUE_SIZE;
	//qp->rear = (new_front + qp->rear -1)  % QUEUE_SIZE;
	qp->size = new_size;
}

//Function to toggle floor request
void toggle_floor_request(Queue* qp,uint8_t floor) {
	// If data exists in an existing queue, find and remove it
	if (is_floor_existed(qp,floor)) {
		remove_floor(qp,floor);
//		set_target_floor(0); // elevator get stop
	}
	else {
		enqueue(qp, floor);
//		set_target_floor(floor);
	}
}

// Peek function to view the front of the queue without removing it
uint8_t peek(Queue* qp) {
	if (!isQueueEmpty(qp)) {
		return qp->floors[qp->front];
	}
	else {
		// Handle queue empty error
		return 0; // Invalid floor
	}
}

uint8_t find_floor_below(uint8_t curr_floor){

}
