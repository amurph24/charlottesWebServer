#include "test_cw_server.h"
#include "test_utils.h"
#include <stdio.h>

int main() {
  int outcome = PASS;
  printf("=================================\n");
  printf("=================================\n");
  printf("running tests\n");

  if (test__get_file_extension() == FAIL) {
    printf("test__get_file_extension failed\n");
    outcome = FAIL;
  }
  if (test__check_resource() == FAIL) {
    printf("test__check_resource failed\n");
    outcome = FAIL;
  }

  printf("=================================\n");
  printf("=================================\n");
  if (outcome == FAIL) {
    printf("some tests failed.\n");
    return 1;
  }
  printf("tests passed.\n");
  return 0;
}
