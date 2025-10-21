#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../customer/customer.h"
#include "stair.h"
#include <unistd.h>
#include <semaphore.h>

typedef struct enterArgs
{
  StairCase_t *s;
  CustomerInfo_t *ci;
} enterArgs;

void change_direction(StairCase_t *s, Direction dir, CustomerInfo_t *ci)
{
  if (dir == UP)
  {
    s->current_direction = UP;
    s->time += s->stair_n;
    ci->exe_time = s->time;
    ci->com_time = ci->exe_time + s->stair_n;
  }
  else
  {
    s->current_direction = DOWN;
    s->time += s->stair_n;
    ci->exe_time = s->time;
    ci->com_time = ci->exe_time + s->stair_n;
  }
}

void *cross(void *args)
{
  enterArgs *e_args = (enterArgs *)args;
  StairCase_t *s = e_args->s;
  CustomerInfo_t *ci = e_args->ci;
  sem_wait(&s->___start);
  if (s->time < ci->arr_time)
  {
    s->time = ci->arr_time;
  }
  if (s->current_direction == NONE)
  {
    s->current_direction = ci->dir;
    ci->exe_time = s->time;
    ci->com_time = ci->exe_time + s->stair_n;
  }
  else if (ci->dir == UP && s->current_direction == DOWN)
  {
    change_direction(s, UP, ci);
  }
  else if (ci->dir == DOWN && s->current_direction == UP)
  {
    change_direction(s, DOWN, ci);
  }
  else
  {
    s->time += 1;
    ci->exe_time = s->time;
    ci->com_time = ci->exe_time + s->stair_n;
  }
  printf(">Customer %d going %s\n", ci->id, ci->dir == UP ? "UP" : "DOWN");
  printf("\tCustomer arrived at time %d\n", ci->arr_time);
  printf("\tStarted at time %d; Completed at stair %d\n", ci->exe_time, ci->com_time);
  sem_post(&s->___start);
  s->completed += 1;
  sleep(s->stair_n);
  pthread_exit(NULL);
}

int enter_fn(void *stair, CustomerInfo_t *cus)
{
  StairCase_t *stair_case = (StairCase_t *)stair;
  enterArgs *args = malloc(sizeof(enterArgs));
  args->s = stair;
  args->ci = cus;
  pthread_create((pthread_t *)&cus->tid, NULL, cross, args);
  return 0;
}

StairCase_t *
cr_stair(int stair_n)
{

  StairCase_t *stair = malloc(sizeof(StairCase_t));
  stair->stair_n = stair_n;
  stair->time = 0;
  stair->completed = 0;
  stair->current_direction = NONE;
  // stair->top_q = create_queue(100);
  // stair->bottom_q = create_queue(100);
  sem_init(&stair->___start, 0, 1);
  stair->enter_stair = enter_fn;
  return stair;
}
