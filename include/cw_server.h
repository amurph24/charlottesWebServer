#ifndef CW_SERVER_H
#define CW_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>

void *cw_handle_request(int sockfd);

void *echo_handle_request(int sockfd);

// for testing

char *_get_file_extension(const char *file_name);

#endif
