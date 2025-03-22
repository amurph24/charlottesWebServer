#include <stdio.h>

#include "launcher.h"

int main(int argc, char** argv){
	printf("arg count: %d, args: %s\n", argc, argv[1]);
	launch_server();
}
