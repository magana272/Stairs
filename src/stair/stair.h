#ifndef STAIR_H
#define STAIR_H
#include "../customer/customer.h"
#include <semaphore.h>
#include "../utils/queue/queue.h"

typedef struct stairs
{
  int stair_n;
  int (*enter_stair)(void *stair, CustomerInfo_t *cus);
  int time;
  int completed;
  Direction current_direction;
  Queue *top_q;
  Customer *bottom_q;
  sem_t ___start;
  sem_t ___SECOND_FLOOR;
  sem_t ___FIRST_FLOOR;
  int current_dir;

} StairCase_t;
StairCase_t *cr_stair(int stair_n);
void *cross(void *args);
#endif