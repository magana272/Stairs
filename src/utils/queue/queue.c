#include <stdlib.h>
#include "queue.h"
#include "../../customer/customer.h"


Queue* create_queue(int capacity) {
  Queue* queue = (Queue*) malloc(sizeof(Queue));
  queue->q = (CustomerInfo_t**) malloc(capacity * sizeof(CustomerInfo_t*));
  queue->front = 0;
  queue->rear = -1;
  queue->current_size = 0;
  return queue;
}

CustomerInfo_t* dequeue(Queue* queue) {
  if (is_empty(queue)) {
    return NULL;
  }
  CustomerInfo_t* item = queue->q[queue->front];
  queue->front += 1;
  queue->current_size -= 1;
  return item;
}
void enqueue(Queue* queue, CustomerInfo_t* item) {
  queue->rear += 1;
  queue->q[queue->rear] = item;
  queue->current_size += 1;
}
int current_size(Queue* queue) {
  return queue->current_size;
}
int is_empty(Queue* queue) {
  return queue->current_size == 0;
}
int peek_time(Queue* queue) {
  if (is_empty(queue)) {
    return -1;
  }
  return ((CustomerInfo_t*)queue->q[queue->front])->arr_time;
}