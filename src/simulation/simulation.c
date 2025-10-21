#include "./simulation.h"
#include "../customer/customer.h"
#include "../stair/stair.h"
#include "../utils/log/logger.h"
#include "../scheduler/scheduler.h"
#include <stdlib.h>
#include <stdio.h>



void
run_fn(Simulation_t* self){
  Simulation_t* s  = (Simulation_t*) self;
  int n_customers = s->num_customers;
  int n_stair = s->num_stairs;
  int i;

  for(i=0; i < n_customers; i++){
    sem_post(&s->stairs->___start);
  }
  for(i=0; i < n_customers;i++){
    s->stairs->enter_stair(s->stairs, s->c_info[i]);
  }
  for(i=0; i < n_customers; i++){
    int r = pthread_join((pthread_t)(self->c_info[i]->tid), 0);
  }
}




Simulation_t*
cr_simu(int num_customer, int num_stairs){
  /*
 * create_simulation(int num_customer, int num_stairs)
 */
  printf("Creating Sim\n");
  Direction dirs[1] = {DOWN};
  CustomerInfo_t** customer_list_info = cr_customer_list(num_customer, DOWN);
  StairCase_t* stair_case = cr_stair(num_stairs);
  Simulation_t* sim = (Simulation_t*) malloc(sizeof(Simulation_t));
  sim->gl_time = 0;
  sim->num_customers = num_customer;
  sim->num_stairs = num_stairs;
  sim->c_info = customer_list_info;
  sim->stairs = stair_case;
  sim->run = (void (*)(struct Simulation_t *))run_fn;
  sim->calatt = calculate_avg_tt;
  sim->n_going_up = rand()%num_customer;
  sim->n_going_down = num_customer - sim->n_going_up;
  return sim;
}

int
calculate_avg_tt(struct Simulation_t* self){
  Simulation_t* s = self;
  int n = self->num_customers;
  int i;
  float turnaround_time = 0.0;
  float response_time = 0.0;
  for(i=0; i < self->num_customers; i++){
    if (s->c_info[i] == NULL){
      printf("Error: Customer %d is NULL\n", i);
      continue;

    }
    turnaround_time += s->c_info[i]->com_time - s->c_info[i]->arr_time;
    response_time += s->c_info[i]->exe_time - s->c_info[i]->arr_time;
  }

  printf("\tAverage Turnaround Time: %.2f\n", turnaround_time / n);
  printf("\tAverage Response Time: %.2f\n", response_time / n);
  return 0;
}

void
run_fn_default(Simulation_t* self){
  struct scheduler_thread_args* args = malloc(sizeof(struct scheduler_thread_args));
  args->scheduler = self->scheduler;
  self->scheduler->run(self->scheduler, simple_scheduler, args);
  pthread_join(self->scheduler->Scheduler_thread, 0);
}


void
RR_default(Simulation_t* self){
  struct scheduler_thread_args* args = malloc(sizeof(struct scheduler_thread_args));
  args->scheduler = self->scheduler;
  self->scheduler->run(self->scheduler, thread_RoundRobin_scheduler, args);
  pthread_join(self->scheduler->Scheduler_thread, 0);
}

Simulation_t* default_sim(int number, int stair, void(*run_run)(Simulation_t*), schedule_type st){
  StairCase_t* sc;
  Simulation_t* sim = malloc(sizeof(Simulation_t));
  sim->scheduler = cr_scheduler(number, stair, sim);
  sc = cr_stair(stair);
  sim->gl_time = 0;
  sim->num_customers = number;
  sim->num_stairs = stair;
  sim->c_info = (CustomerInfo_t*) malloc(sizeof(CustomerInfo_t) * number);
  sim->stairs = sc;

  if (st == FIFO){
    sim->run = run_fn_default;
  }else if (st == ROUND_ROBIN){
    sim->run = RR_default;
  }
  sim->calatt = calculate_avg_tt;
  sim->n_going_up = rand() % number;
  sim->n_going_down = number - sim->n_going_up;
  sim->lger = cr_logger();
  return sim;
}