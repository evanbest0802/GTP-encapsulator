#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
	char	sendline[1500];
	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5202);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	//str_cli(stdin, sockfd);		/* do it all */


	while (fgets(sendline, 1500, stdin) != NULL) {

		write(sockfd, sendline, strlen(sendline));
		printf("packet send\n");
	}
	exit(0);
}
