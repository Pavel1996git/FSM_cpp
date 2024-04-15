/*
 * Queue_my.c
 *
 *  Created on: Apr 10, 2024
 *      Author: pavel
 */
#include"Queue_my.h"

/*
void initQueue(volatile Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

volatile arrayType isEmpty(volatile Queue q)
{
    //return (q->size == 0);
	return (q.size == 0);
}

int isFull(volatile Queue *q)
{
    return (q->size == MAX_SIZE);
}

void enqueue(volatile Queue *q, arrayType data)
{
    if (isFull(q)) {

        return;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->array[q->rear] = data;
    q->size++;
}

volatile arrayType dequeue(volatile Queue q)
{
    if (isEmpty(q)) {

        return -1;
    }
  // arrayType data = q->array[q->front];
   // q->front = (q->front + 1) % MAX_SIZE;
   // q->size--;
     arrayType data = q.array[q.front];
      q.front = (q.front + 1) % MAX_SIZE;
      q.size--;
    return data;
}

volatile arrayType peek(volatile Queue q)
{
    if (isEmpty(q)) {

        return -1;
    }
  //  return q->array[q->front];
    return q.array[q.front];
}
*/

void initQueue(volatile Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

volatile arrayType isEmpty(volatile Queue* q)
{
    return (q->size == 0);

}

int isFull(volatile Queue *q)
{
    return (q->size == MAX_SIZE);
}

void enqueue(volatile Queue *q, arrayType data)
{
    if (isFull(q)) {

        return;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->array[q->rear] = data;
    q->size++;
}

volatile arrayType dequeue(volatile Queue* q)
{
    if (isEmpty(q)) {

        return -1;
    }
 arrayType data = q->array[q->front];
    q->front = (q->front + 1) % MAX_SIZE;
    q->size--;

    return data;
}

volatile arrayType peek(volatile Queue* q)
{
    if (isEmpty(q)) {

        return -1;
    }
   return q->array[q->front];

}

