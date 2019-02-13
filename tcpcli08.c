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
int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
	char	sendline[1500];
	if (argc != 2)
	{
		printf("usage: tcpcli <IPaddress>");
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5202);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	//str_cli(stdin, sockfd);		/* do it all */


	while (fgets(sendline, 1500, stdin) != NULL) {

		write(sockfd, sendline, strlen(sendline));
		printf("packet send\n");
	}
	exit(0);
}
