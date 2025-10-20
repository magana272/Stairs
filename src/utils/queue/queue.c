#include <stdlib.h>
#include "queue.h"
#include "../../customer/customer.h"


Queue* create_queue(int capacity) {
  Queue* queue = (Queue*) malloc(sizeof(Queue));
  queue->q = (void**) malloc(capacity * sizeof(void*));
  queue->front = 0;
  queue->rear = -1;
  queue->current_size = 0;
  return queue;
}

void* dequeue(Queue* queue) {
  if (queue->front > queue->rear) {
    return NULL;
  }
  queue->current_size -= 1;
  return queue->q[queue->front++];
}
void enqueue(Queue* queue, void* item) {
  queue->q[++queue->rear] = item;
  queue->current_size += 1;
}
int current_size(Queue* queue) {
  return queue->current_size;
}