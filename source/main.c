#include <stdio.h>
#include <stdlib.h>

#include "launcher.h"

int main(int argc, char** argv){
	if (argc != 2) {
		printf("usage: %s <port>", argv[1]);
		return 1;
	}
	// TODO: check port is actually an int
	launch_server(strtol(argv[1], NULL, 0));
}
