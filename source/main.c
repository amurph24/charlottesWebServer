#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "launcher.h"

int is_number(const char *number) {
  int i = 0;
  if (number[i] == '-')
    i++;
  for (; number[i] != 0; i++) {
    if (!isdigit(number[i]))
      return -1;
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s <port>\n", argv[1]);
    return 1;
  }

  if (is_number(argv[1])) {
    printf("<port> must be a number\n");
    return 1;
  }
  // TODO: accept dir as arg, only resources in this dir can be requested
  launch_server(strtol(argv[1], NULL, 0));
}
