#ifndef LOGGER_H
#define LOGGER_H

typedef struct Logger{
  void (*get_usage)();
  void (*log_info)(const char* message);
  void (*print_arrival_times)(const char* direction, int* arrival_times, int n);
  // void (*print_arrival_array)(const char* direction, int* arrival_array, int n);
} Logger_t;

Logger_t* cr_logger();
void usage();
void log_info(const char* message);
void print_arrival_times(const char* direction, int* arrival_times, int n);

#endif