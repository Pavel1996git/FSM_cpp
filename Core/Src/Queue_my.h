/*
 * Queue_my.h
 *
 *  Created on: Apr 10, 2024
 *      Author: pavel
 */

#ifndef INC_QUEUE_MY_H_
#define INC_QUEUE_MY_H_

#include "stdint.h"
#include "stdio.h"
#define MAX_SIZE 35
#define DELAY_QUEUE 15000
typedef int8_t arrayType;

typedef struct {
	arrayType array[MAX_SIZE];
    int front;
    int rear;
    int size;
} Queue;
/*
void initQueue(volatile  Queue *q);


volatile arrayType isEmpty(volatile  Queue q);


void enqueue(volatile  Queue *q, arrayType data);


volatile arrayType dequeue(volatile Queue q);


volatile arrayType peek(volatile Queue q);
*/

void initQueue(volatile  Queue *q);


volatile arrayType isEmpty(volatile  Queue* q);


void enqueue(volatile  Queue *q, arrayType data);


volatile arrayType dequeue(volatile Queue* q);


volatile arrayType peek(volatile Queue* q);


#endif /* INC_QUEUE_MY_H_ */
