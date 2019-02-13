#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
	char	sendline[1500];
	if (argc != 2)
		err_quit("usage: udpcli <IPaddress>");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5202);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	//dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));
	

	while (fgets(sendline, 1500, stdin) != NULL) {

		sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

		//n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

		//recvline[n] = 0;	/* null terminate */
		//Fputs(recvline, stdout);
		printf("packet send\n");
	}
	exit(0);
}
