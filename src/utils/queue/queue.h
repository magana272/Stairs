#ifndef QUEUE_H
#define QUEUE_H
#include "../../customer/customer.h"
typedef struct Queue{
  CustomerInfo_t** q;
  int front;
  int rear;
  int current_size;
} Queue;
Queue* create_queue(int capacity);
CustomerInfo_t* dequeue(Queue* queue);
void enqueue(Queue* queue, CustomerInfo_t* item);
int current_size(Queue* queue);
int is_empty(Queue* queue);
int peek_time(Queue* queue);

#endif