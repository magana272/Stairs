#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include "customer.h"

// typedef pthread_t Customer;

CustomerInfo_t* cr_customer(void *fn, int arr_time, Direction dir){
  CustomerInfo_t* cinf = malloc(sizeof(CustomerInfo_t));
  Customer* customer_id =  (Customer*) malloc(sizeof(Customer));
  cinf->dir = dir;
  cinf->tid = customer_id;
  cinf->arr_time = arr_time;
  cinf->id = rand() %100;
  return cinf;
}

CustomerInfo_t** cr_customer_list(int length, Direction dir){
  CustomerInfo_t** customer_list = malloc(length * sizeof(struct CustomerInfo_t));
  for(int i=0; i < length; i++){
    customer_list[i] = cr_customer(NULL, i, dir);
  }

  return customer_list;
}