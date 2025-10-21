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


  printf("\n\n");
  printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("ROUND_ROBIN: Creating Simulation with %d customers and %d stairs\n", n, s);
  printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  Simulation_t* simu = default_sim(n,s, run_fn_default, ROUND_ROBIN);
  simu->scheduler->quanta = 5;
  simu->run(simu);
  printf("\t==============================\n");
  simu->calatt(simu);
  printf("\t==============================\n");
  printf("\n\n");
  printf("++++++++++++++++++++++++++++++++++++\n");
  printf("FIFO: Creating Simulation with %d customers and %d stairs\n", n, s);
  printf("++++++++++++++++++++++++++++++++++++\n");
  simu = default_sim(n,s, run_fn_default, FIFO);
  simu->scheduler->quanta = 5;
  simu->run(simu);
  printf("==============================\n");
  simu->calatt(simu);
  printf("==============================\n");




}