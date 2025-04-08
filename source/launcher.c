#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"
#include "launcher.h"
#include "cw_server.h"

//TODO: move to a header file, or fetch from user or system
#define IP "10.0.0.180"
#define QUEUE_LIMIT 20


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

	// handlers for special ports
	int (*handle_request)(int sockfd, struct sockaddr_in sock_addr, socklen_t sock_addr_len);
	switch (port) {
		//case 7: handle_request = echo_handle_request;
		default: handle_request = cw_handle_request;
	}
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
