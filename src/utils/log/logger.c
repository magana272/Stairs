
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

void
usage(){
  printf("usage: stairs -n num  -s num\n");
  printf("-n    The number of customers\n");
  printf("-s    The number of Stairs\n");
}

Logger_t*
cr_logger(){
  Logger_t* l = malloc(sizeof(Logger_t));
  l->get_usage = &usage;
  return l;
}


