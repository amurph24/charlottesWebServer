#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define INTERFACE "wlp3s0"

// caller must initialise host to contain ip address object
// suggest using size NI_MAXHOST
char* get_self_ip(char* host)
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                                           host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
		freeifaddrs(ifaddr);
                exit(EXIT_FAILURE);
            }
	    if (strcmp(ifa->ifa_name, INTERFACE)) continue;
        }
    }
    freeifaddrs(ifaddr);
    return host;
}

int main () {
	char host[NI_MAXHOST];
	get_self_ip(host);
	printf("host ip: %s\n", host);
	return 0;
}
