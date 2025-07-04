#include "cw_server.h"  // SAFE, UNSAFE
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

void _check_resource_tester(char *resource_string, int permitted,
                            int *outcome) {
  if (_check_resource(resource_string) != permitted) {
    printf("X");
    printf("\nrequested resource '%s' hasn't been correctly flagged as "
           "safe/unsafe.\n",
           resource_string);
    *outcome = FAIL;
  } else {
    printf(".");
  }
  return;
}

int test__check_resource() {
  int outcome = PASS;
  char *resource_var = "something.java";

  _check_resource_tester("script.py", SAFE, &outcome);
  _check_resource_tester(resource_var, SAFE, &outcome);

  // test unsafe files
  _check_resource_tester("../aunt_file.cpp", UNSAFE, &outcome);
  _check_resource_tester("/lib/root.sh", UNSAFE, &outcome);

  printf("\n");

  return outcome;
}
