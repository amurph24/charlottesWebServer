#ifndef CW_SERVER_H
#define CW_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>

int cw_handle_request(int sockfd);

int echo_handle_request(int sockfd);

// for testing

char *_get_file_extension(const char *file_name);

#endif
