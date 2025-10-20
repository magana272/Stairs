#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "argparser.h"
#include "../../utils/log/logger.h"

void argument_parser(int argc, char *argv[], int* n, int* s){
  int opt;
  while ((opt = getopt(argc, argv, "n:s:")) != -1) {
        switch (opt) {
        case 'n':
            if(!isdigit(optarg[0])){
              usage();
              exit(1);
            }
            *n = atoi(optarg);
            break;
        case 's':
            if(!isdigit(optarg[0])){
              usage();
              exit(1);
            }
            *s = atoi(optarg);
            break;
        default:
          exit(1);
        }
      }

};