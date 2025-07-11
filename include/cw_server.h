#ifndef CW_SERVER_H
#define CW_SERVER_H

#define SAFE 0
#define UNSAFE -1

#include <netinet/in.h>
#include <sys/socket.h>

void *cw_handle_request(void *sockfd_pointer);

void *echo_handle_request(void *sockfd_pointer);

// for testing

char *_get_file_extension(const char *file_name);

int _check_resource(const char *resource_string);

#endif
