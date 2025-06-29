#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void error_and_die(char *message) {
  perror(message);
  exit(1);
}
