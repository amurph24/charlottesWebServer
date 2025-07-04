#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <pthread.h>

#include "common.h"
#include "cw_server.h"
#include "launcher.h"

// TODO: move to a header file, or fetch from user or system
#define QUEUE_LIMIT 20
#define INTERFACE "wlp3s0"

// caller must initialise host to contain ip address object
// suggest using size NI_MAXHOST
int get_self_ip(char *host) {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs failed");
    return -1;
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    family = ifa->ifa_addr->sa_family;

    if (family == AF_INET) {
      s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host,
                      NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if (s != 0) {
        freeifaddrs(ifaddr);
        printf("getnameinfo() failed: %s\n", gai_strerror(s));
        return -1;
      }
      if (!strcmp(ifa->ifa_name, INTERFACE))
        break;
    }
  }
  freeifaddrs(ifaddr);
  return 0;
}

int launch_server(int port) {
  char host_ip[NI_MAXHOST] = {0};
  if (get_self_ip(host_ip) == -1)
    error_and_die("couldn't get self ip address");
  printf("configuring socket %s:%d...\n", host_ip, port);
  struct sockaddr_in server_sock_addr;
  struct in_addr server_ip_addr;
  int server_sock, conn_sock;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0)
    error_and_die("couldn't initialise listener socket\n");

  // convert IP from string to server order format
  if (!inet_aton(host_ip, &server_ip_addr))
    error_and_die("invalid ip address\n");

  // configure server address for socket
  server_sock_addr.sin_family = AF_INET;
  server_sock_addr.sin_port = htons(port);
  server_sock_addr.sin_addr = server_ip_addr;

  // connect socket to server and start listening
  if (bind(server_sock, (struct sockaddr *)&server_sock_addr,
           sizeof(server_sock_addr)) < 0)
    error_and_die("failed to bind socket to server address\n");
  if (listen(server_sock, QUEUE_LIMIT))
    error_and_die("failed to listen on server socket\n");

  printf("success! Listening on port %d...\n", port);

  // handlers for special ports
  void *(*handle_request)(void *sockfd_pointer);
  switch (port) {
  case 10007:
    handle_request = echo_handle_request;
    break;
  default:
    handle_request = cw_handle_request;
  }
  while (1) {
    // details of requesters
    struct sockaddr_in req_sock_addr;
    socklen_t req_addr_len;
    req_addr_len = sizeof(req_sock_addr);

    // TODO: I don't remember why this needs to be here
    fflush(stdout);
    conn_sock =
        accept(server_sock, (struct sockaddr *)&req_sock_addr, &req_addr_len);
    if (conn_sock < 0) {
      perror("failed to connect with requester\n");
      continue;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, handle_request, (void *)(&conn_sock));
    pthread_detach(thread);
  }
  close(server_sock);
  return 0;
}
