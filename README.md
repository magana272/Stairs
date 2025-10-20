
# Scheduler


```C
typedef struct scheduler_t{
  pthread_t scheduler_thread;
  Simulation_t* sim;
  Logger_t* logger;
  time_t start_time;
  time_t end_time;
  Queue* up_queue;
  Queue* down_queue;
  void (*run)();
} scheduler_t;
```

```C
typedef pthread_t Customer;
typedef enum {UP, DOWN} Direction;
typedef struct CustomerInfo_t{
  Customer* tid;
  int id;
  Direction dir;
  time_t arr_time;
  time_t com_time;
  time_t exe_time;
}CustomerInfo_t;
```

```C
typedef struct stairs {
  int stair_n;
  int (*enter_stair)(void* stair, CustomerInfo_t* cus);
  Customer* top_q;
  Customer* bottom_q;
  sem_t ___start;
  sem_t ___SECOND_FLOOR;
  sem_t ___FIRST_FLOOR;
}StairCase_t;
```

```C
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
  scheduler_t* scheduler;

  int (*calatt)(struct Simulation_t* self);
  void (*run)(struct Simulation_t* self, void* args);
  void (*test_ptr_func)(struct Simulation_t* self);

}Simulation_t;
```



