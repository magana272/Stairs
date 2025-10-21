
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

void usage()
{
  printf("usage: stairs -n num  -s num\n");
  printf("-n    The number of customers\n");
  printf("-s    The number of Stairs\n");
}

void log_info(const char *message)
{
  printf("LOG: %s\n", message);
}

void print_arrival_times(const char *direction, int *arrival_times, int n)
{
  printf("Arrival times %s: ", direction);
  for (int i = 0; i < n; i++)
  {
    printf("%d ", arrival_times[i]);
  }
  printf("\n");
}
Logger_t *
cr_logger()
{
  Logger_t *l = malloc(sizeof(Logger_t));
  l->get_usage = usage;
  l->log_info = log_info;
  l->print_arrival_times = print_arrival_times;
  return l;
}
