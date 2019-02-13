//#include	"unp.h"
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>	
#include	<arpa/inet.h>	
#include	<errno.h>
#include	<fcntl.h>		
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	
#include	<sys/uio.h>		
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>
#include	<ifaddrs.h>
int
main(int argc, char **argv)
{
	int sockfd,udpfd,n;
	
	struct sockaddr_in	cliaddr, servaddr,remoteaddr,localaddr;
	char line[1500],peeraddress[INET_ADDRSTRLEN];
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(5202);

	bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

	for(;;)
	{
		socklen_t clilen = sizeof(cliaddr);
		n = recvfrom(sockfd, line,1500, 0, (struct sockaddr*)&cliaddr, &clilen);
		printf("after read\n");
		udpfd = socket(AF_INET, SOCK_DGRAM, 0);
		bzero(&remoteaddr, sizeof(remoteaddr));
		remoteaddr.sin_family = AF_INET;
		remoteaddr.sin_port = htons(5202);
		inet_pton(AF_INET, "127.0.0.15", &remoteaddr.sin_addr);
		printf("%d\n",setsockopt(udpfd,SOL_SOCKET,SO_BINDTODEVICE,"enp0s9",6));
		sendto(udpfd,line,strlen(line),0,(struct sockaddr*)&remoteaddr,sizeof(remoteaddr));
		bzero(&line, strlen(line));
		close(udpfd);
	}
}
