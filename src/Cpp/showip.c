/*
 * showip.c -- show IP addresses for a host given on the command line
 *
 * created at Mar 23, 2012
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main(int argc, char* argv[])
{
    int status;
    char ipstr[INET6_ADDRSTRLEN];
    struct addrinfo hints, *res = NULL, *p = NULL;

    if (argc != 2)
    {
    	fprintf(stderr, "usage: showip hostname\n");
    	return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
    	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    	exit(1);
    }

    printf("IP addresses for %s:\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next)
    {
    	const void*     addr;
    	const char*     ipver;

    	if (p->ai_family == AF_INET)
    	{
            ipver = "IPv4";
    		struct sockaddr_in* ipv4 = (struct sockaddr_in*)(p->ai_addr);
    		addr = &(ipv4->sin_addr);

    	}
    	else
    	{
            ipver = "IPv6";
    		struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
    		addr = &(ipv6->sin6_addr);
    	}

    	inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
    	printf("    %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);
    printf("\n");

	return 0;
}