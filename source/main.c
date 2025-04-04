#include <stdio.h>
#include <stdlib.h>

#include "launcher.h"

int main(int argc, char** argv){
	if (argc != 2) {
		printf("usage: %s <port>", argv[1]);
		return 1;
	}
	// TODO: accept dir as arg, only resources in this dir can be requested
	// TODO: check port is actually an int
	launch_server(strtol(argv[1], NULL, 0));
}
