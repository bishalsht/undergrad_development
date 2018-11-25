#include "/home/amit/Desktop/Dropbox/sockets/socket_world.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int rsfd, n;
	char buffer[8192];

	if ((rsfd = socket (AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
		err_sys ("Error", errno);

	while ((n = read (rsfd, buffer, sizeof (buffer))) > 0)
	{
		buffer[n] = 0;
		printf("Caught packet: %s\n", buffer + sizeof (struct tcphdr) + sizeof (struct iphdr));
		write (1, buffer, sizeof (buffer));
	}
	return 0;
}