#include <stdlib.h>
#include<unistd.h>
#include <stdio.h>
#include "../customer/customer.h"
#include "../utils/queue/queue.h"
#include "scheduler.h"




Scheduler_t* cr_scheduler(int num_customer, int num_stairs, Simulation_t* simu) {

  Scheduler_t* scheduler = (Scheduler_t*) malloc(sizeof(Scheduler_t));
  scheduler->Scheduler_thread = (pthread_t) malloc(sizeof(pthread_t));
  scheduler->sim = simu;
  scheduler->start_time = 0;
  scheduler->end_time = 0;
  scheduler->up_queue = create_queue(num_customer);
  scheduler->down_queue = create_queue(num_customer);
  scheduler->run = start_scheduler;
  return scheduler;
}

void start_scheduler(Scheduler_t* scheduler, void* thread_func, void* thread_args) {

  scheduler->sim->lger->log_info("Starting Scheduler");
  pthread_create(&(scheduler->Scheduler_thread), NULL, thread_func, thread_args);
  scheduler->start_time = 0;
}

void stop_scheduler(Scheduler_t* scheduler) {
  scheduler->end_time = time(NULL);
}

int* arrival_times_from_scheduler_start(int n, int start){
  int* arrival_time = malloc(n * sizeof(int));
  int t =  start;
  for (int i=0; i < n; i++){
    arrival_time[i] = t + (rand() % 3 );
    t = arrival_time[i];
  }
  return arrival_time;

}

int* arrival_times_all_AT_time(int n, int t){
  int* arrival_time = malloc(n * sizeof(int));
  for (int i=0; i < n; i++){
    arrival_time[i] = t;
  }
  return arrival_time;

}


int* arrival_times(int n){
  // From 0
  int* arrival_time = malloc(n * sizeof(int));
  int time = 0;
  for (int i=0; i < n; i++){
    arrival_time[i] = time + (rand() % 3 );
    time = arrival_time[i];
  }
  return arrival_time;
}

int next_jobs(int *arrival_times, int current_time, int til, int n, int curr_job){
  for (int i=curr_job; i < n; i++){
    if(current_time >= arrival_times[i]  && arrival_times[i]<= til){
        return i;
    }
  }
  return -1;
}

void* thread_RoundRobin_scheduler(void* arg){
  struct scheduler_thread_args* args = arg;
  Scheduler_t* scheduler = args->scheduler;
  StairCase_t* stair = scheduler->sim->stairs;
  Logger_t* logger = scheduler->sim->lger;
  int quanta = scheduler->sim->scheduler->quanta;

  CustomerInfo_t** cu = malloc(scheduler->sim->n_going_up * sizeof(CustomerInfo_t*));
  CustomerInfo_t** cd = malloc(scheduler->sim->n_going_down * sizeof(CustomerInfo_t*));
  int* arrival_time = arrival_times_from_scheduler_start(scheduler->sim->n_going_up, 0);
  int* arrival_time_down = arrival_times_from_scheduler_start(scheduler->sim->n_going_down, 0);
  int u_idx=0;
  int i_idx=0;
  int next = 0;
  int rounds = 1;
  int time = 0;

  for (int i=0; i < scheduler->sim->n_going_up; i++){
    cu[i] = cr_customer_with_id(NULL, arrival_time[i], UP, i);
  }
  for (int i=0; i < scheduler->sim->n_going_down; i++){
    cd[i] = cr_customer_with_id(NULL, arrival_time_down[i], DOWN, i + scheduler->sim->n_going_up);
  }
  logger->print_arrival_times("UP", arrival_time, scheduler->sim->n_going_up);
  logger->print_arrival_times("DOWN", arrival_time_down, scheduler->sim->n_going_down);

  int current_time = 0;
  int indx_at_start = 0;

  for (int i=0; i < scheduler->sim->n_going_up ; i++){
    scheduler->sim->c_info[i] = cu[i];
  }
  for (int i=0; i < scheduler->sim->n_going_down; i++){
    scheduler->sim->c_info[i + scheduler->sim->n_going_up] = cd[i];
  }



  Queue* thread_q = create_queue(30);
  while(scheduler->sim->stairs->completed != scheduler->sim->num_customers){
    current_time = ((rounds -1) * quanta )+1;
    while(current_time == 0 || (current_time % quanta) !=0){
      indx_at_start = u_idx;
      next = next_jobs(arrival_time, current_time,  (rounds * quanta), scheduler->sim->n_going_up, u_idx);
      if (next == -1){
        current_time+=1;
      }else{
        scheduler->sim->stairs->enter_stair(scheduler->sim->stairs, cu[u_idx]);
        enqueue(thread_q, cu[u_idx]);
        u_idx +=1;
        }
    }

    while(!is_empty(thread_q)){
        Customer * c = dequeue(thread_q);
        pthread_join((pthread_t)*c , NULL);
    }
    rounds+=1;
    current_time = ((rounds -1) * quanta )+1;
    while((current_time % quanta) % quanta !=0){
      next = next_jobs(arrival_time, current_time,  (rounds * quanta), scheduler->sim->n_going_down, i_idx);
      if (next == -1){
        current_time+=1;
      }else{
        scheduler->sim->stairs->enter_stair(scheduler->sim->stairs, cd[i_idx]);
        enqueue(thread_q, cd[i_idx]);
        i_idx+=1;
      }
      }
    while(!is_empty(thread_q)){
        Customer * c = dequeue(thread_q);
        pthread_join((pthread_t)*c , NULL);
    }
  }

}

void* simple_scheduler(void* arg){

  struct scheduler_thread_args* args = arg;
  Scheduler_t* scheduler = args->scheduler;
  StairCase_t* stair = scheduler->sim->stairs;
  Logger_t* logger = scheduler->sim->lger;
  CustomerInfo_t** customers = malloc(scheduler->sim->num_customers * sizeof(CustomerInfo_t*));
  int* arrival_time = arrival_times(scheduler->sim->num_customers);
  logger->print_arrival_times("RANDOM", arrival_time, scheduler->sim->num_customers);
  for (int i=0; i < scheduler->sim->num_customers; i++){
    Direction dir = (i % 2 == 0) ? UP : DOWN;
    customers[i] = cr_customer(NULL, arrival_time[i], dir);
    scheduler->sim->stairs->enter_stair(scheduler->sim->stairs, customers[i]);
    pthread_join((pthread_t)(customers[i]->tid), NULL);
  }
  for (int i=0; i < scheduler->sim->num_customers; i++){
    scheduler->sim->c_info[i] = customers[i];
  }


}
