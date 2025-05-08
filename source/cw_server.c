#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "common.h"
#include "cw_server.h"

#define REQUEST_BUFFER_LEN 4096
#define RESPONSE_BUFFER_LEN 242880

const char *_get_file_extension(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
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
    } else if (strcasecmp(file_ext, "jpg") == 0 || strcasecmp(file_ext, "jpeg") == 0) {
        return "image/jpeg";
    } else if (strcasecmp(file_ext, "png") == 0) {
        return "image/png";
    } else {
        return "application/octet-stream";
    }
}

int _validate_request() {
	//TODO
	return 0;
}

char* _extract_resource(char* request_string) {
	char* resource_name;
	resource_name = strtok(request_string, " ");
	resource_name = strtok(NULL, " ");
	return resource_name;
}

int build_http_response(const char *file_name, 
                        char *response, 
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
	while ((bytes_read = read(file_fd, 
                            response + *response_len, 
                            RESPONSE_BUFFER_LEN - *response_len)) > 0) {
		*response_len += bytes_read;
	}
build_http_close_file:
	close(file_fd);
build_http_free_header:
	free(header);
	return 0;
}

int cw_handle_request(int sockfd, struct sockaddr_in sock_addr, socklen_t sock_addr_len) {
	char *return_buff = (char*)malloc(RESPONSE_BUFFER_LEN*sizeof(char));
	if (return_buff == NULL) {
		perror("couldn't allocate reponse buffer");
		goto cleanup_cw_handle_request;
	}

	ssize_t num_bytes_received;
	char request_buffer[REQUEST_BUFFER_LEN] = {0};

	num_bytes_received = recv(sockfd, request_buffer, REQUEST_BUFFER_LEN, 0);
	if (num_bytes_received < 0) {
		perror("couldn't receive bytes from client\n");
		goto cleanup_cw_handle_request;
	}
	if (_validate_request() < 0) {
		printf("received bad request, exiting\n");
		//TODO: give the user some sort of response
		goto cleanup_cw_handle_request;
	}

	// parse request
	char request_buffer_copy[REQUEST_BUFFER_LEN] = {0};
	strncpy(request_buffer_copy, request_buffer, REQUEST_BUFFER_LEN*sizeof(char));
	char* resource_name_str = _extract_resource(request_buffer_copy);
	printf("requested resource: %s \n", resource_name_str);

	char* ip_addr = inet_ntoa(sock_addr.sin_addr);
	printf("requester ip: %s\n", ip_addr);
	printf("addr length: %d\n", sock_addr_len);
	
	// assemble and return response
	size_t response_len = RESPONSE_BUFFER_LEN*sizeof(char);
	if (build_http_response(resource_name_str+1, return_buff, &response_len) < 0) {
		perror("couldn't build response");
		goto cleanup_cw_handle_request;
	}
	if (write(sockfd, return_buff, strlen(return_buff)) < 0) {
		perror("failed to communicate");
		printf("failed to write to %s", ip_addr);
		goto cleanup_cw_handle_request;
	}

cleanup_cw_handle_request:
	free(return_buff);

	return 0;
}
