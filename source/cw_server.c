#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "common.h"
#include "cw_server.h"

#define REQUEST_BUFFER_LEN 4096

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

int cw_handle_request(int sockfd, struct sockaddr_in sock_addr, socklen_t sock_addr_len) {
	uint8_t return_buff[4096+1];
	ssize_t num_bytes_received;
	char request_buffer[REQUEST_BUFFER_LEN] = {0};

	num_bytes_received = recv(sockfd, request_buffer, REQUEST_BUFFER_LEN, 0);
	if (num_bytes_received < 0) {
		perror("couldn't receive bytes from client\n");
		return -1;
	}
	if (_validate_request() < 0) {
		printf("received bad request, exiting\n");
		//TODO: give the user some sort of response
		return -1;
	}
	printf("request: %s\n", request_buffer);

	// parse request
	char request_buffer_copy[REQUEST_BUFFER_LEN] = {0};
	strncpy(request_buffer_copy, request_buffer, REQUEST_BUFFER_LEN*sizeof(char));
	char* resource_name_str = _extract_resource(request_buffer_copy);
	printf("requested resource: %s \n", resource_name_str);

	char* ip_addr = inet_ntoa(sock_addr.sin_addr);
	printf("requester ip: %s\n", ip_addr);
	printf("addr length: %d\n", sock_addr_len);
	
	// assemble and return response
	snprintf((char*)return_buff, sizeof(return_buff), "HTTP/1.0 200 OK\r\n\r\nHello");
	if (write(sockfd, (char*)return_buff, strlen((char*)return_buff)) < 0) {
		perror("failed to communicate");
		printf("failed to write to %s", ip_addr);
		return 1;
	}

	return 0;
}
