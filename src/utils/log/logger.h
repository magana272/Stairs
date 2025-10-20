#ifndef LOGGER_H
#define LOGGER_H

typedef struct Logger{
  void (*get_usage)();
} Logger_t;

Logger_t* cr_logger();
void usage();


#endif