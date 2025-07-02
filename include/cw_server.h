#ifndef CW_SERVER_H
#define CW_SERVER_H

#include <sys/socket.h>

int cw_handle_request(int sockfd, struct sockaddr_in sock_addr,
                      socklen_t sock_addr_len);

int echo_handle_request(int sockfd, struct sockaddr_in sock_addr,
                        socklen_t sock_addr_len);

#endif
