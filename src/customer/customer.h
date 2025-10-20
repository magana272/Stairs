#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <pthread.h>
#include <time.h>

typedef pthread_t Customer;

typedef enum {UP, DOWN} Direction;

typedef struct CustomerInfo_t{
  Customer* tid;
  int id;
  Direction dir;
  int arr_time;
  int com_time;
  int exe_time;
}CustomerInfo_t;
CustomerInfo_t* cr_customer(void *fn, int arr_time, Direction dir);
CustomerInfo_t** cr_customer_list(int length, Direction dir);
#endif