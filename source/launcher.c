#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//TODO: move to a header file, or fetch from user or system
#define IP "10.0.0.180"
#define QUEUE_LIMIT 20

int launch_server(int port) {
	printf("configuring socket %s:%d...\n", IP, port);
	struct sockaddr_in server_sock_addr;
	struct in_addr server_ip_addr;
	int server_sock, conn_sock;

	uint8_t buff[4096+1];

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0) {
		printf("failed to instatiate listener server socket\n");
		return 1;
	}

	// convert IP from string to server order format
	if (!inet_aton(IP, &server_ip_addr)) {
		printf("invalid IP address provided for listener\n");
		return 1;
	}

	// configure server address for socket
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(port);
	server_sock_addr.sin_addr = server_ip_addr;

	// connect socket to server and start listening
	if (bind(server_sock, (struct sockaddr *) &server_sock_addr, sizeof(server_sock_addr)) < 0) {
		printf("failed to bind socket to server address\n");
		return 1;
	}
	if (listen(server_sock, QUEUE_LIMIT)) {
		printf("failed to listen on server socket\n");
		return 1;
	}

	printf("success! Listening on port %d...\n", port);
	while (1) {
		// details of requesters
		struct sockaddr_in req_sock_addr;
		socklen_t req_addr_len;
		req_addr_len = sizeof(req_sock_addr);

		fflush(stdout);	
		conn_sock = accept(server_sock, (struct sockaddr *) &req_sock_addr, &req_addr_len);
		if (conn_sock < 0) {
			printf("failed to connect with requester\n");
			return 1; // maybe this should be a continue?
		}

		char* ip_addr = inet_ntoa(req_sock_addr.sin_addr);
		printf("requester ip: %s\n", ip_addr);
		
		snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");

		write(conn_sock, (char*)buff, strlen((char*)buff));
		close(conn_sock);
	}
	return 0;
}
