#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
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
	char* request_buffer = (char*)malloc(REQUEST_BUFFER_LEN*(sizeof(char)));

	recv(sockfd, request_buffer, REQUEST_BUFFER_LEN, 0); //TODO: replace 0 with MSG_WAITALL -> requires handshake
	if (_validate_request() < 0) {
		printf("received bad request, exiting\n");
		//TODO: give the user some sort of response
		return 1;
	}
	printf("request: %s\n", request_buffer);

	// parse request
	char* request_buffer_copy = (char*)malloc(4096*sizeof(char));
	strcpy(request_buffer_copy, request_buffer);
	char* resource_name_str = _extract_resource(request_buffer_copy);
	printf("requested resource: %s \n", resource_name_str);
	free(request_buffer_copy);

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

	free(request_buffer);
	return 0;
}
