#include "cw_server.h"
#include "test_utils.h" // PASS, FAIL
#include <stdio.h>
#include <string.h>

void _get_file_extension_tester(char *filename, char *correct_ext,
                                int *outcome) {
  if (strcmp(_get_file_extension(filename), correct_ext)) {
    printf("X");
    printf("\nThe file ext for '%s' is not '%s'\n", filename, correct_ext);
    *outcome = FAIL;
  } else {
    printf(".");
  }
  return;
}

int test__get_file_extension() {
  int outcome = PASS;
  char *filename_var = "something.c";

  _get_file_extension_tester("something.csv", "csv", &outcome);
  _get_file_extension_tester("something.html", "html", &outcome);
  _get_file_extension_tester(filename_var, "c", &outcome);

  // test files without extensions
  _get_file_extension_tester("something.", "", &outcome);
  _get_file_extension_tester("something", "", &outcome);

  printf("\n");

  return outcome;
}
