#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <pthread.h>

#include "common.h"
#include "cw_server.h"

#define REQUEST_BUFFER_LEN 16384
#define RESPONSE_BUFFER_LEN 242880

#define DEFAULT_RESOURCE "index.html\0"

char *_get_file_extension(const char *file_name) {
  char *dot = strrchr(file_name, '.');
  if (!dot || dot == file_name) {
    return "";
  }
  return dot + 1;
}

const char *_get_mime_type(const char *file_ext) {
  if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0) {
    return "text/html";
  } else if (strcasecmp(file_ext, "txt") == 0) {
    return "text/plain";
  } else if (strcasecmp(file_ext, "jpg") == 0 ||
             strcasecmp(file_ext, "jpeg") == 0) {
    return "image/jpeg";
  } else if (strcasecmp(file_ext, "png") == 0) {
    return "image/png";
  } else {
    return "application/octet-stream";
  }
}

int _validate_request() {
  // TODO
  return 0;
}

// check for nefarious resource requests, return -1 if suspicious
int _check_resource(const char *resource_string) {
  if (resource_string[0] == '/')
    return UNSAFE;
  if (strstr(resource_string, "../") != NULL)
    return UNSAFE;
  if (strstr(resource_string, "~/"))
    return UNSAFE;
  return SAFE;
}

char *_extract_resource(char *request_string) {
  char *resource_name;
  resource_name = strtok(request_string, " ");
  resource_name = strtok(NULL, " ");
  return resource_name;
}

int build_http_response(const char *file_name, char *response,
                        size_t *response_len) {
  // build HTTP header
  const char *mime_type = _get_mime_type(_get_file_extension(file_name));
  char *header = (char *)malloc(RESPONSE_BUFFER_LEN * sizeof(char));
  if (header == NULL) {
    perror("failed to allocated header, returning");
    return -1;
  }

  snprintf(header, RESPONSE_BUFFER_LEN,
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: %s\r\n"
           "\r\n",
           mime_type);

  // file doesn't exist, response is 404 Not Found
  int file_fd = open(file_name, O_RDONLY);
  if (file_fd == -1) {
    snprintf(response, RESPONSE_BUFFER_LEN,
             "HTTP/1.1 404 Not Found\r\n"
             "Content-Type: text/plain\r\n"
             "\r\n"
             "404 Not Found");
    *response_len = strlen(response);
    goto build_http_free_header;
  }

  // get file size for Content-Length
  struct stat file_stat;
  fstat(file_fd, &file_stat);

  // copy header to response buffer
  *response_len = 0;
  memcpy(response, header, strlen(header));
  *response_len += strlen(header);

  // copy file to response buffer
  ssize_t bytes_read;
  while ((bytes_read = read(file_fd, response + *response_len,
                            RESPONSE_BUFFER_LEN - *response_len)) > 0) {
    *response_len += bytes_read;
  }
  /*
build_http_close_file:
  close(file_fd);
  */
build_http_free_header:
  free(header);
  return 0;
}

void *cw_handle_request(void *sockfd_pointer) {
  int sockfd = *((int *)sockfd_pointer);
  // TODO : put this stuff in a separate function
  struct sockaddr_in sock_addr;
  socklen_t sock_addr_len;
  sock_addr_len = sizeof(sock_addr);

  getpeername(sockfd, (struct sockaddr *)&sock_addr, &sock_addr_len);
  //

  char *ip_addr = inet_ntoa(sock_addr.sin_addr);
  printf("requester ip: %s\n", ip_addr);
  printf("addr length: %d\n", sock_addr_len);

  char *return_buff = (char *)malloc(RESPONSE_BUFFER_LEN * sizeof(char));
  if (return_buff == NULL) {
    perror("couldn't allocate reponse buffer");
    goto close_conn;
  }

  ssize_t num_bytes_received;
  char request_buffer[REQUEST_BUFFER_LEN] = {0};

  num_bytes_received = recv(sockfd, request_buffer, REQUEST_BUFFER_LEN - 1, 0);
  if (num_bytes_received < 0) {
    perror("couldn't receive bytes from client\n");
    goto cleanup_cw_handle_request;
  }
  request_buffer[num_bytes_received] = '\0';
  if (_validate_request() < 0) {
    printf("received bad request, exiting\n");
    // TODO: give the user some sort of response
    goto cleanup_cw_handle_request;
  }

  // parse request
  char request_buffer_copy[REQUEST_BUFFER_LEN] = {0};
  strncpy(request_buffer_copy, request_buffer,
          REQUEST_BUFFER_LEN * sizeof(char));
  char *resource_name_str = _extract_resource(request_buffer_copy);

  // go to homepage if nothing requested
  if (resource_name_str[0] == '/' && resource_name_str[1] == '\0')
    resource_name_str = DEFAULT_RESOURCE;
  else
    resource_name_str++;

  if (_check_resource(resource_name_str) != SAFE) {
    printf("naughty request from '%s', ", ip_addr);
    printf("%s\n", resource_name_str);
    goto cleanup_cw_handle_request;
  }

  // assemble and return response
  size_t response_len = (RESPONSE_BUFFER_LEN - 1) * sizeof(char);
  if (build_http_response(resource_name_str, return_buff, &response_len) < 0) {
    perror("couldn't build response");
    goto cleanup_cw_handle_request;
  }
  return_buff[response_len] = '\0';

  if (write(sockfd, return_buff, strlen(return_buff)) < 0) {
    perror("failed to communicate");
    printf("failed to write to %s", ip_addr);
    goto cleanup_cw_handle_request;
  }

cleanup_cw_handle_request:
  free(return_buff);

close_conn:
  close(sockfd);

  printf("------\n");
  pthread_exit(NULL);
}

void *echo_handle_request(void *sockfd_pointer) {
  int sockfd = *((int *)sockfd_pointer);
  // TODO : put this stuff in a separate function
  struct sockaddr_in sock_addr;
  socklen_t sock_addr_len;
  sock_addr_len = sizeof(sock_addr);

  getpeername(sockfd, (struct sockaddr *)&sock_addr, &sock_addr_len);
  //
  char *ip_addr = inet_ntoa(sock_addr.sin_addr);
  printf("requester ip: %s\n", ip_addr);
  printf("addr length: %d\n", sock_addr_len);

  ssize_t num_bytes_received;
  char request_buffer[REQUEST_BUFFER_LEN] = {0};

  num_bytes_received = recv(sockfd, request_buffer, REQUEST_BUFFER_LEN - 1, 0);
  if (num_bytes_received < 0) {
    perror("couldn't receive bytes from client\n");
    close(sockfd);
    pthread_exit(NULL);
  }
  request_buffer[num_bytes_received] = '\0';
  if (write(sockfd, request_buffer, strlen(request_buffer)) < 0) {
    perror("failed to communicate");
    printf("failed to write to %s", ip_addr);
  }
  printf("------\n");

  close(sockfd);
  pthread_exit(NULL);
}
