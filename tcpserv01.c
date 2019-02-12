#include	"unp.h"
#include	<ifaddrs.h>
int
main(int argc, char **argv)
{
	int					listenfd, connfd,loopbackfd,n;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr,loopbackaddr,localaddr;
	char		line[MAXLINE],peeraddress[INET_ADDRSTRLEN];
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(5202);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
		inet_ntop(AF_INET, &cliaddr.sin_addr,peeraddress,INET_ADDRSTRLEN);
		if(!strcmp("127.0.0.1",peeraddress))
		{
			printf("loopback address\n");
			Close(connfd);
			continue;
		}
		if ( (childpid = Fork()) == 0) 
		{	/* child process */
			Close(listenfd);	/* close listening socket */
			//str_echo(connfd);	/* process the request */
			printf("in for\n");
			again:
			while((n=Read(connfd,line,MAXLINE))>0)
			{
				struct ifaddrs * ifaddrstruct = NULL;
				void * tmpaddrptr = NULL;
				char addressBuffer[INET_ADDRSTRLEN];
				getifaddrs(&ifaddrstruct);
				while(ifaddrstruct != NULL)
				{	
					if(ifaddrstruct->ifa_addr->sa_family==AF_INET)
					{
						tmpaddrptr=&((struct sockaddr_in *)ifaddrstruct->ifa_addr)->sin_addr;
						
						inet_ntop(AF_INET, tmpaddrptr, addressBuffer, INET_ADDRSTRLEN);
						if(!strcmp(ifaddrstruct->ifa_name,"enp0s9"))break;
					}
					ifaddrstruct=ifaddrstruct->ifa_next;
				}
				printf("after read\n");
				loopbackfd = Socket(AF_INET, SOCK_STREAM, 0);
				bzero(&loopbackaddr, sizeof(loopbackaddr));
				loopbackaddr.sin_family = AF_INET;
				loopbackaddr.sin_port = htons(5202);
				Inet_pton(AF_INET, "8.8.8.8", &loopbackaddr.sin_addr);
				Inet_pton(AF_INET, addressBuffer, &localaddr.sin_addr);
				bind(loopbackfd,(SA *) &localaddr, sizeof(localaddr));
				//printf("%d\n",setsockopt(loopbackfd,SOL_SOCKET,SO_BINDTODEVICE,"lo",2));
				printf("before connect\n");
				Connect(loopbackfd, (SA *) &loopbackaddr, sizeof(loopbackaddr));
				printf("after\n");
				Write(loopbackfd, line, strlen(line));
				bzero(&line, strlen(line));
				Close(loopbackfd);
			}
			if(n<0 && errno==EINTR)goto again;
			else if (n<0) printf("errorr\n");
			printf("child exit\n");
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}
