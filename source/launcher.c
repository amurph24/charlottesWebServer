#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

//TODO: move to a header file, or fetch from user or system
#define IP "209.52.108.102"
#define QUEUE_LIMIT 20

int validate_request() {
	//TODO
	return 0;
}

char* extract_resource(char* request_string) {
	char* resource_name;
	resource_name = strtok(request_string, " ");
	resource_name = strtok(NULL, " ");
	return resource_name;
}

#include <stdlib.h>
#define REQUEST_BUFFER_LEN 4096
int handle_request(int sockfd, struct sockaddr_in sock_addr, socklen_t sock_addr_len) {
	uint8_t return_buff[4096+1];
	char* request_buffer = (char*)malloc(REQUEST_BUFFER_LEN*(sizeof(char)));

	recv(sockfd, request_buffer, REQUEST_BUFFER_LEN, 0); //TODO: replace 0 with MSG_WAITALL -> requires handshake
	if (validate_request() < 0) {
		printf("received bad request, exiting\n");
		//TODO: give the user some sort of response
		return 1;
	}
	printf("request: %s\n", request_buffer);

	// parse request
	char* request_buffer_copy = (char*)malloc(4096*sizeof(char));
	strcpy(request_buffer_copy, request_buffer);
	char* resource_name_str = extract_resource(request_buffer_copy);
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

int launch_server(int port) {
	printf("configuring socket %s:%d...\n", IP, port);
	struct sockaddr_in server_sock_addr;
	struct in_addr server_ip_addr;
	int server_sock, conn_sock;


	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
		error_and_die("couldn't initialise listener socket\n");

	// convert IP from string to server order format
	if (!inet_aton(IP, &server_ip_addr))
		error_and_die("invalid ip address\n");

	// configure server address for socket
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(port);
	server_sock_addr.sin_addr = server_ip_addr;

	// connect socket to server and start listening
	if (bind(server_sock, (struct sockaddr *) &server_sock_addr, sizeof(server_sock_addr)) < 0)
		error_and_die("failed to bind socket to server address\n");
	if (listen(server_sock, QUEUE_LIMIT))
		error_and_die("failed to listen on server socket\n");

	printf("success! Listening on port %d...\n", port);
	while (1) {
		// details of requesters
		struct sockaddr_in req_sock_addr;
		socklen_t req_addr_len;
		req_addr_len = sizeof(req_sock_addr);

		fflush(stdout);	
		conn_sock = accept(server_sock, (struct sockaddr *) &req_sock_addr, &req_addr_len);
		if (conn_sock < 0) {
			perror("failed to connect with requester\n");
			continue;
		}

		handle_request(conn_sock, req_sock_addr, req_addr_len);
		close(conn_sock);
	}
	return 0;
}
