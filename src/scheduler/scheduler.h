#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "../simulation/simulation.h"
#include "../stair/stair.h"
#include "../utils/log/logger.h"
#include "../customer/customer.h"
#include "../utils/queue/queue.h"

typedef struct scheduler
{
  pthread_t *Scheduler_thread;
  struct Simulation_t *sim;
  Logger_t *logger;
  time_t start_time;
  time_t end_time;
  Queue *up_queue;
  Queue *down_queue;
  void (*run)();
  int quanta;
} Scheduler_t;

struct scheduler_thread_args
{
  struct scheduler *scheduler;
  int quanta;
};

Scheduler_t *cr_scheduler(int num_customer, int num_stairs, struct Simulation_t *simu);
void start_scheduler(Scheduler_t *scheduler, void *thread_func, void *thread_args);
void stop_scheduler(Scheduler_t *scheduler);
int schedule(Scheduler_t *scheduler, Direction dir);

void *thread_static_scheduler(void *arg);
void *thread_FIFO_scheduler(void *arg);
void *simple_scheduler(void *arg);
void *thread_RoundRobin_scheduler(void *arg);
void *thread_random_arrival_REALTIME(void *arg);
int *arrival_times(int n);
int next_jobs(int *arrival_times, int current_time, int til, int n, int curr_job);

#endif