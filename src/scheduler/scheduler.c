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
  //

  printf("Starting Scheduler\n");
  pthread_create(&(scheduler->Scheduler_thread), NULL, thread_func, thread_args);
  scheduler->start_time = 0;
}

void stop_scheduler(Scheduler_t* scheduler) {
  scheduler->end_time = time(NULL);
}
// void* thread_static_scheduler(void* arg) {
//   struct thread_args* args = (struct thread_args*) arg;
//   Scheduler_t* scheduler = args->scheduler;
//   int n_going_down = scheduler->sim->n_going_down;
//   int n_going_up = scheduler->sim->n_going_up;
//   Queue* up_queue = create_queue(n_going_up);
//   Queue* down_queue = create_queue(n_going_down);
//   int time = 0;
//   while(1){
//     if (current_size(up_queue) < n_going_up){
//       enqueue(up_queue, cr_customer(NULL,
//         scheduler->start_time + time + 1,
//         UP));
//     }
//     if (current_size(down_queue) < n_going_down){
//       enqueue(down_queue, cr_customer(NULL,
//         scheduler->start_time + time + 1,
//         DOWN));
//     }
//     time += 1;
//     }
//     while(current_size(down_queue) > 0){
//       CustomerInfo_t* cd = (CustomerInfo_t*) dequeue(down_queue);
//       scheduler->sim->stairs->enter_stair(
//         scheduler->sim->stairs,
//         cd
//       );
//     }

// }
// void* thread_FIFOscheduler(void* arg) {
//   struct thread_args* args = arg;
//   Scheduler_t* scheduler = args->scheduler;
//   int n_up=0;
//   int n_down=0;
//   while(1){
//     sleep(random()%5);
//     if (n_up != scheduler->sim->n_going_up){
//       if (rand()%100 > 50){
//             scheduler->sim->stairs->enter_stair(
//               scheduler->sim->stairs,
//               cr_customer(NULL,
//               (long)time(NULL) - scheduler->start_time,
//               UP)
//             );
//             n_up += 1;
//       }
//     }
//     if (n_down != scheduler->sim->n_going_down){
//       if (rand()%100 > 50){
//             scheduler->sim->stairs->enter_stair(
//               scheduler->sim->stairs,
//               cr_customer(NULL,
//               (long)time(NULL) - scheduler->start_time,
//               DOWN)
//             );
//       }
//     }
//   }
// }

int* arrival_times_from_scheduler_start(int n, int start){
  int* arrival_time = malloc(n * sizeof(int));
  int t =  start;
  for (int i=0; i < n; i++){
    arrival_time[i] = t + (rand() % 3);
    t = arrival_time[i];
  }
  return arrival_time;

}


int* arrival_times(int n){
  // From 0
  int* arrival_time = malloc(n * sizeof(int));
  int time = 0;
  for (int i=0; i < n; i++){
    arrival_time[i] = time + rand()%3 ;
    time = arrival_time[i];
  }
  return arrival_time;
}

int next_jobs(int *arrival_times, int current_time, int til, int n, int curr_job){

  for (int i=curr_job; i < n; i++){
    if(current_time > arrival_times[i]  && arrival_times[i]<= til){
        return i;
    }
  }
  return -1;
}

void* thread_RoundRobin_scheduler(void* arg){
  // printf("thread_running?");
  struct scheduler_thread_args* args = arg;
  Scheduler_t* scheduler = args->scheduler;
  int quanta = args->quanta;
  int time = 0;
  CustomerInfo_t** cu = malloc(scheduler->sim->n_going_up * sizeof(CustomerInfo_t*));
  CustomerInfo_t** cd = malloc(scheduler->sim->n_going_down * sizeof(CustomerInfo_t*));
  int* arrival_time = arrival_times_from_scheduler_start(scheduler->sim->n_going_up, 0);
  int* arrival_time_down = arrival_times_from_scheduler_start(scheduler->sim->n_going_down, 0);
  int u_idx=0;
  int i_idx=0;
  int next = 0;
  int rounds = 1;
  printf("Arrival times UP: ");
  for(int i=0; i < scheduler->sim->n_going_up; i++){
    printf("%d ", arrival_time[i]);
  }
  printf("\n");
  printf("Arrival times DOWN: ");
  for(int i=0; i < scheduler->sim->n_going_down; i++){
    printf("%d ", arrival_time_down[i]);
  }
  printf("\n");


  int current_time = 0;
  int indx_at_start =0;

  while(scheduler->sim->completed !=  scheduler->sim->num_customers){
    current_time = (rounds -1) * quanta;
    while(current_time == 0 || (current_time % quanta) !=0){
      next = next_jobs(arrival_time, current_time,  (rounds * quanta), scheduler->sim->n_going_up, u_idx);
      if (next == -1){
        current_time+=1;
      }else{
        cu[u_idx] = cr_customer(NULL, arrival_time[u_idx], UP);
        cu[u_idx]->exe_time = current_time;
        scheduler->sim->stairs->enter_stair(scheduler->sim->stairs, cu[u_idx]);
        pthread_join((pthread_t)cu[u_idx]->tid, 0);
        scheduler->sim->completed+=1;
        u_idx=next;
        u_idx +=1;
        }
      }
    rounds+=1;
    // Downward Customers
    current_time = (rounds-1) * quanta + 1;

    printf("Round %d Downward Customers\n", rounds);
    printf("Current Time: %d\n", current_time);
    while((current_time % quanta) % quanta !=0){
      next = next_jobs(arrival_time, current_time,  (rounds * quanta), scheduler->sim->n_going_down, i_idx);
      if (next == -1){
        current_time+=1;
      }else{
        cd[i_idx] = cr_customer(NULL, arrival_time_down[i_idx], DOWN);
        cd[i_idx]->exe_time = current_time;
        scheduler->sim->stairs->enter_stair(scheduler->sim->stairs, cd[i_idx]);
        pthread_join((pthread_t)cd[i_idx]->tid, 0);
        scheduler->sim->completed+=1;
        i_idx=next;
        i_idx+=1;
      }
    }
  }
}