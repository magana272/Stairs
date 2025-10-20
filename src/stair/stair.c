#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../customer/customer.h"
#include "stair.h"
#include <unistd.h>
#include <semaphore.h>


typedef struct enterArgs{
  StairCase_t* s;
  CustomerInfo_t* ci;
}enterArgs;

void* cross(void* args){
  // printf("Crossing Function Called\n");
  enterArgs* e_args  = (enterArgs*) args;
  StairCase_t* s = e_args->s;
  CustomerInfo_t* ci = e_args->ci;

  char* dirstr;
  switch(ci->dir){
    case (UP):
      dirstr = "UP";
      break;
    default:
      dirstr = "DOWN";
  };


  if (dirstr == "UP"){
    sem_wait(&s->___start);
    sem_wait(&s->___SECOND_FLOOR);
    sleep(s->stair_n);
    sem_post(&s->___start);
    printf("Customer %d going %s stairs at time %d, and arrived at %d \n", ci->id, dirstr, (int)ci->exe_time, (int)ci->arr_time);
    sem_post(&s->___SECOND_FLOOR);
    ci->com_time  = ci->exe_time + s->stair_n;
    printf("Customer %d Completed at time %ld\n", ci->id, (int)ci->com_time);
    printf("TurnAroundTime: %d \n", (int)ci->com_time - (int)ci->arr_time);

  }
  else{
    sem_wait(&s->___start);
    sem_wait(&s->___FIRST_FLOOR);
    sleep(s->stair_n);
    sem_post(&s->___FIRST_FLOOR);
    printf("Customer %d going %s stairs at time %ld\n", ci->id, dirstr, (int)ci->arr_time);
    sem_post(&s->___SECOND_FLOOR);
    ci->com_time  = ci->exe_time + s->stair_n;
    printf("Customer %d Completed at time %d\n", ci->id, (int)ci->com_time);
    printf("TurnAroundTime: %ld \n", (int)ci->com_time - (int)ci->arr_time);

  }


}


int enter_fn(void *stair, CustomerInfo_t *cus){
  StairCase_t* stair_case = (StairCase_t*) stair;
  enterArgs* args = malloc(sizeof(enterArgs));
  args->s = stair;
  args->ci = cus;
  pthread_create((pthread_t*)&cus->tid, NULL, cross, args);
  return 0;
}

/**
 * create_stair_case(int stair_n) - Brief description of foobar.
 * @arg: Description of argument of foobar.
 *
 * Longer description of foobar.
 *
 * Return: A Locked Stair Case
 */
StairCase_t*
cr_stair(int stair_n)
{

  /*
 * This is the preferred style for multi-line
 * comments in the Linux kernel source code.
 * Please use it consistently.
 *
 * Description:  A column of asterisks on the left side,
 * with beginning and ending almost-blank lines.
 */
  StairCase_t* stair =  malloc(sizeof(StairCase_t));
  stair->stair_n = stair_n;
  sem_init(&stair->___start, 0, stair_n);
  sem_init(&stair->___SECOND_FLOOR, 0, 1);
  sem_init(&stair->___FIRST_FLOOR, 0, 1);
  stair->enter_stair = enter_fn;
  return stair;
}



