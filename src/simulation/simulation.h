#ifndef SIMULATION_H
#define SIMULATION_H
#include <pthread.h>
#include "../stair/stair.h"
#include "../customer/customer.h"
#include "../utils/log/logger.h"
#include "../scheduler/scheduler.h"


typedef enum schedule_type{FIFO, ROUND_ROBIN} schedule_type;

typedef struct Simulation_t{
  unsigned int gl_time;
  unsigned int num_customers;
  unsigned int num_stairs;
  unsigned int n_going_up;
  unsigned int n_going_down;
  unsigned int completed;
  CustomerInfo_t** c_info;
  Logger_t* lger;
  StairCase_t* stairs;
  struct scheduler* scheduler;

  int (*calatt)(struct Simulation_t* self);
  void (*run)(struct Simulation_t* self);
  void (*test_ptr_func)(struct Simulation_t* self);

}Simulation_t;

Simulation_t* cr_simu_test(void);
Simulation_t* cr_simu(int num_customer, int num_stairs);
Simulation_t* cr_simu_bi_dir(int num_customer, int num_stairs);
Simulation_t* default_sim(int number, int stair, void(*run_run)(Simulation_t*), schedule_type st);
// Types of schedulers


void run_fn(Simulation_t* self);
void run_fn_defualt(Simulation_t* self);
void print_adder(struct Simulation_t* self);
int calculate_avg_tt(struct Simulation_t* self);
#endif