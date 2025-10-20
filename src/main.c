#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "./utils/log/logger.h"
#include "../simulation/simulation.h"
#include "../scheduler/scheduler.h"
#include "utils/argparser/argparser.h"

#include <assert.h>



int main(int argc, char *argv[]){
  int n;
  int s;
  if (argc != 5){
    usage();
    exit(-1);
  }
  argument_parser(argc, argv, &n, &s);
  printf("n: %d, s: %d\n", n, s);


  // Simulation_t* sim = cr_simu(n, s);
  // sim->run(sim);


  // Validate Scheduler Works
  //
  Simulation_t* simu = default_sim(n,s, run_fn_defualt, ROUND_ROBIN);


  simu->run(simu);

  // int* arrival_times[5] = {0,1,3,4,5};
  // int next_jobs(int *arrival_times, int current_time, int quanta, int n, int curr_job);
  // int curr_index = 0;
  // int next;
  // for(int current_time=0; current_time < 6; current_time++){
  //   printf("Current Time: %d\n", current_time);
  //   printf("curr_index: %d\n", curr_index);
  //   next = next_jobs(arrival_times, current_time, 5, 5, curr_index);
  //   if(next==-1){
  //     printf("No job at time %d\n", current_time);

  //   }
  //   else{
  //     printf("Job %d at time %d, arrival time: %d\n", next, current_time, arrival_times[curr_index]);
  //     curr_index = next;
  //     curr_index+=1;
  //   }
  // }




}