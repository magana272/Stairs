#ifndef QUEUE_H
#define QUEUE_H
typedef struct Queue{
  void** q;
  int front;
  int rear;
  int current_size;
} Queue;
Queue* create_queue(int capacity);
void* dequeue(Queue* queue);
void enqueue(Queue* queue, void* item);
int current_size(Queue* queue);
#endif