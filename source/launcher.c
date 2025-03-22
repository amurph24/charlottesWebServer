#include <stdio.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

//TODO: move to a header file, or fetch from user or system
#define PORT 8000
#define IP "10.0.0.180"
#define QUEUE_LIMIT 20

int launch_server() {
	printf("configuring socket %s:%d...\n", IP, PORT);
	struct sockaddr_in server_sock_addr;
	struct in_addr server_ip_addr;

	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0) {
		printf("failed to instatiate listener server socket");
		return 1;
	}

	// convert IP from string to server order format
	if (!inet_aton(IP, &server_ip_addr)) {
		printf("invalid IP address provided for listener");
		return 1;
	}

	// configure server address for socket
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(PORT);
	server_sock_addr.sin_addr = server_ip_addr;

	// connect socket to server and start listening
	if (bind(server_sock, (struct sockaddr *) &server_sock_addr, sizeof(server_sock_addr)) < 0) {
		printf("failed to bind socket to server address");
		return 1;
	}
	if (listen(server_sock, QUEUE_LIMIT)) {
		printf("failed to listen on server socket");
		return 1;
	}

	printf("success! Listening on port %d...", PORT);
	while (1) {
	
	}
	printf("success!\n");
	return 0;
}
