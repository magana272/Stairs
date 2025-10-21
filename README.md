
# Staircase

```C
// Author.h

typedef struct init_group {
const * created_by = "Manuel Magana";
const * ProjectName = "stair";
const * groupname = "init_group";

}init_group;

```
# Overview
```text

1. Your group number, and the name of the students in your group

      Group Name: init_group
      Group Members: Manuel

2. A short description of the project.

    A staircase simulator. Schedulers passes customers to stairs. Schedulers in charge of organizing customers. Stairs in charges of moving customers and time.


3. Explain how you implemented the project:


    There are a lot of functions implemented. However the main function and data structures
    are listed down below.

    The main idea: We have a scheduler that gives the stair customers.
    We have a stair that allow for customers to move in the same direction. The stairs prevents
    Customers from moving in opposite directions.

4. Explain how you tested your project and list the test cases you used.

  - Simpler functions were tested with unit tests.
  - Tested classes were interacting as expected. i.e: Allowing interchangeability of the the scheduler types.
  - Tested various N and S.

5. Explain how you guarantee that your code is free of deadlock and starvation.
  - This is enforced by the scheduler. The scheduler waits for customers moving up to finish before allowing customers of the opposite direction to pass.
  - Starvation is prevented also by the scheduler. The interval or quanta is chosen to be 5. This makes sure that one direction isn't starved.


6. Find the average Turnaround time of the examples you run, and explain using these performance measures how you adjusted your project to make your design “efficient”.

  - Provided below in Example output section.

  - ( ROUND_ROBIN: Creating Simulation with 10 customers and 5 stairs ) -- This is my efficient solution.
  - FIFO is included to demonstrate a bad solution.

```




### Core Components

- **Simulation (`simulation.c`)**: Main simulation engine that coordinates all components
- **Scheduler (`scheduler.c`)**: Manages customer queues and implements scheduling algorithms. **Responsible for placing Customers On Stairs**
- **Customer (`customer.c`)**: Represents individual customers with arrival times and directions
- **Staircase (`stair.c`)**: Models the physical staircase with semaphore-based access control
- **Queue (`queue.c`)**: Generic queue implementation for customer management
- **Logger (`logger.c`)**: Logging utility for debugging and analysis

### Key Data Structures

```c
// Customer representation
typedef struct CustomerInfo_t {
    Customer *tid;          // Thread ID
    int id;                 // Customer ID
    Direction dir;          // UP or DOWN
    int arr_time;           // Arrival time
    int com_time;           // Completion time
    int exe_time;           // Execution time
} CustomerInfo_t;

// Staircase with concurrency control
typedef struct stairs {
    int stair_n;            // Number of stairs
    Direction current_direction;
    sem_t ___start;         // Semaphores for synchronization

} StairCase_t;

// Simulation state
typedef struct Simulation_t {
    unsigned int num_customers;
    unsigned int num_stairs;
    unsigned int n_going_up;
    unsigned int n_going_down;
    CustomerInfo_t **c_info;
    StairCase_t *stairs;
    struct scheduler *scheduler;
} Simulation_t;
```

## Building and Running

### Build

```bash
make
```

### Run

```bash
./run.sh

# Or with parameters
./build/main -n <number_of_customers> -s <number_of_stairs>
```

### Command Line Arguments

- `-n <number>`: Number of customers in the simulation
- `-s <number>`: Number of stairs in the staircase

### Example

```bash
./build/main -n 10 -s 5
```

This runs a simulation with 10 customers and 5 stairs.

## Scheduling Algorithms

### FIFO (First In, First Out)

- Customers are served in the order they arrive
- Simple and fair but may not be optimal for throughput

### Round Robin

- Scheduler takes customers within an interval and allows them to pass on the stairs
- i.e. All customers that arrive within an the interval [0-5] are allowed to enter the stairs. After the 5 time units the direction changes.Customers that are moving down and arrive in time units [6-10] are allowed to pass.

## Development

### Project Structure

```text
├── src/
│   ├── main.c                 # Entry point
│   ├── simulation/            # Simulation engine
│   ├── scheduler/             # Scheduling algorithms
│   ├── customer/              # Customer management
│   ├── stair/                 # Staircase implementation
│   └── utils/                 # Utilities
│       ├── argparser/         # Command line parsing
│       ├── log/               # Logging system
│       └── queue/             # Queue data structure
├── build/                     # Build artifacts
├── makefile                   # Build configuration
└── README.md                  # This file
```

### Key Functions

- `cr_stair()`: Initializes a staircase structure
- `void *thread_RoundRobin_scheduler(void *arg)`: Main logic for Round Robin Scheduling
- `default_sim()`: Creates a simulation with specified parameters
- `run_fn_default()`: Default simulation execution function
- `calculate_avg_tt()`: Calculates average turnaround time
- `cr_customer_list()`: Creates a list of customers
- `cr_stair()`: Initializes a staircase structure


## Performance Metrics

The simulation tracks and reports:

- Average Response time
- Average turnaround time
- Customer completion times
- Response time and Turnaround time per customer

## Vagrant Support

A `Vagrantfile` is included for Ubuntu development environments:

```bash
vagrant up
vagrant ssh
```

## Example:

```bash
./build/main -n 10 -s 5
n: 10, s: 5


++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ROUND_ROBIN: Creating Simulation with 10 customers and 5 stairs
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LOG: Starting Scheduler
Arrival times UP: 1 1 2
Arrival times DOWN: 2 3 4 4 4 5 7
>Customer 0 going UP
        Customer arrived at time 1
        Started at time 1; Completed at stair 6
>Customer 1 going UP
        Customer arrived at time 1
        Started at time 2; Completed at stair 7
>Customer 2 going UP
        Customer arrived at time 2
        Started at time 3; Completed at stair 8
>Customer 3 going DOWN
        Customer arrived at time 2
        Started at time 8; Completed at stair 13
>Customer 4 going DOWN
        Customer arrived at time 3
        Started at time 9; Completed at stair 14
>Customer 5 going DOWN
        Customer arrived at time 4
        Started at time 10; Completed at stair 15
>Customer 7 going DOWN
        Customer arrived at time 4
        Started at time 11; Completed at stair 16
>Customer 8 going DOWN
        Customer arrived at time 5
        Started at time 12; Completed at stair 17
>Customer 6 going DOWN
        Customer arrived at time 4
        Started at time 13; Completed at stair 18
>Customer 9 going DOWN
        Customer arrived at time 7
        Started at time 14; Completed at stair 19
        ==============================
Customer 0: Arrival Time: 1, Execution Time: 1, Completion Time: 6
        Turnaround Time: 5
        Response Time: 0
Customer 1: Arrival Time: 1, Execution Time: 2, Completion Time: 7
        Turnaround Time: 6
        Response Time: 1
Customer 2: Arrival Time: 2, Execution Time: 3, Completion Time: 8
        Turnaround Time: 6
        Response Time: 1
Customer 3: Arrival Time: 2, Execution Time: 8, Completion Time: 13
        Turnaround Time: 11
        Response Time: 6
Customer 4: Arrival Time: 3, Execution Time: 9, Completion Time: 14
        Turnaround Time: 11
        Response Time: 6
Customer 5: Arrival Time: 4, Execution Time: 10, Completion Time: 15
        Turnaround Time: 11
        Response Time: 6
Customer 6: Arrival Time: 4, Execution Time: 13, Completion Time: 18
        Turnaround Time: 14
        Response Time: 9
Customer 7: Arrival Time: 4, Execution Time: 11, Completion Time: 16
        Turnaround Time: 12
        Response Time: 7
Customer 8: Arrival Time: 5, Execution Time: 12, Completion Time: 17
        Turnaround Time: 12
        Response Time: 7
Customer 9: Arrival Time: 7, Execution Time: 14, Completion Time: 19
        Turnaround Time: 12
        Response Time: 7
        Average Turnaround Time: 10.00
        Average Response Time: 5.00
        ==============================


++++++++++++++++++++++++++++++++++++
FIFO: Creating Simulation with 10 customers and 5 stairs
++++++++++++++++++++++++++++++++++++
LOG: Starting Scheduler
Arrival times RANDOM: 2 3 5 6 6 6 7 8 10 12
>Customer 67 going UP
        Customer arrived at time 2
        Started at time 2; Completed at stair 7
>Customer 29 going DOWN
        Customer arrived at time 3
        Started at time 8; Completed at stair 13
>Customer 82 going UP
        Customer arrived at time 5
        Started at time 13; Completed at stair 18
>Customer 30 going DOWN
        Customer arrived at time 6
        Started at time 18; Completed at stair 23
>Customer 62 going UP
        Customer arrived at time 6
        Started at time 23; Completed at stair 28
>Customer 23 going DOWN
        Customer arrived at time 6
        Started at time 28; Completed at stair 33
>Customer 67 going UP
        Customer arrived at time 7
        Started at time 33; Completed at stair 38
>Customer 35 going DOWN
        Customer arrived at time 8
        Started at time 38; Completed at stair 43
>Customer 29 going UP
        Customer arrived at time 10
        Started at time 43; Completed at stair 48
>Customer 2 going DOWN
        Customer arrived at time 12
        Started at time 48; Completed at stair 53
==============================
Customer 67: Arrival Time: 2, Execution Time: 2, Completion Time: 7
        Turnaround Time: 5
        Response Time: 0
Customer 29: Arrival Time: 3, Execution Time: 8, Completion Time: 13
        Turnaround Time: 10
        Response Time: 5
Customer 82: Arrival Time: 5, Execution Time: 13, Completion Time: 18
        Turnaround Time: 13
        Response Time: 8
Customer 30: Arrival Time: 6, Execution Time: 18, Completion Time: 23
        Turnaround Time: 17
        Response Time: 12
Customer 62: Arrival Time: 6, Execution Time: 23, Completion Time: 28
        Turnaround Time: 22
        Response Time: 17
Customer 23: Arrival Time: 6, Execution Time: 28, Completion Time: 33
        Turnaround Time: 27
        Response Time: 22
Customer 67: Arrival Time: 7, Execution Time: 33, Completion Time: 38
        Turnaround Time: 31
        Response Time: 26
Customer 35: Arrival Time: 8, Execution Time: 38, Completion Time: 43
        Turnaround Time: 35
        Response Time: 30
Customer 29: Arrival Time: 10, Execution Time: 43, Completion Time: 48
        Turnaround Time: 38
        Response Time: 33
Customer 2: Arrival Time: 12, Execution Time: 48, Completion Time: 53
        Turnaround Time: 41
        Response Time: 36
        Average Turnaround Time: 23.90
        Average Response Time: 18.90
==============================
```
