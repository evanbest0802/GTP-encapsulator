//#include	"unp.h"
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>	
#include	<arpa/inet.h>	
#include	<errno.h>
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
	int					listenfd, connfd,udpfd,n;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr,remoteaddr,localaddr;
	char		line[1500],peeraddress[INET_ADDRSTRLEN];
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(5202);

	bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

	//listen(listenfd, 100);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen);
		inet_ntop(AF_INET, &cliaddr.sin_addr,peeraddress,INET_ADDRSTRLEN);
		if(!strcmp("127.0.0.1",peeraddress))
		{
			printf("loopback address\n");
			close(connfd);
			continue;
		}
		if ( (childpid = fork()) == 0) 
		{	/* child process */
			close(listenfd);	/* close listening socket */
			//str_echo(connfd);	/* process the request */
			again:
			while((n=read(connfd,line,1500))>0)
			{
				/*struct ifaddrs * ifaddrstruct = NULL;
				void * tmpaddrptr = NULL;
				char addressBuffer[INET_ADDRSTRLEN];
				getifaddrs(&ifaddrstruct);
				while(ifaddrstruct != NULL)
				{
					if((ifaddrstruct->ifa_addr->sa_family)==AF_INET)
					{
						printf("dsafasdfasfasdfasdfaf\n");
						tmpaddrptr=&((struct sockaddr_in *)ifaddrstruct->ifa_addr)->sin_addr;						
						inet_ntop(AF_INET, tmpaddrptr, addressBuffer, INET_ADDRSTRLEN);
						if(!strcmp(ifaddrstruct->ifa_name,"enp0s9")){printf("got it\n");break;}
							
					}printf("find next\n");
					ifaddrstruct=ifaddrstruct->ifa_next;
					printf("point to next\n");
				}*/
				printf("after read\n");
				udpfd = socket(AF_INET, SOCK_DGRAM, 0);
				bzero(&remoteaddr, sizeof(remoteaddr));
				remoteaddr.sin_family = AF_INET;
				remoteaddr.sin_port = htons(5202);
				inet_pton(AF_INET, "127.0.0.15", &remoteaddr.sin_addr);
				//Inet_pton(AF_INET, addressBuffer, &localaddr.sin_addr);
				//bind(udpfd,(SA *) &localaddr, sizeof(localaddr));
				printf("%d\n",setsockopt(udpfd,SOL_SOCKET,SO_BINDTODEVICE,"enp0s9",6));
				//printf("before connect\n");
				//Connect(loopbackfd, (SA *) &remoteaddr, sizeof(remoteaddr));
				//printf("after\n");
				//Write(udpfd, line, strlen(line));
				sendto(udpfd,line,strlen(line),0,(struct sockaddr*)&remoteaddr,sizeof(remoteaddr));
				bzero(&line, strlen(line));
				close(udpfd);
			}
			if(n<0 && errno==EINTR)goto again;
			else if (n<0) printf("errorr\n");
			exit(0);
		}
		close(connfd);			/* parent closes connected socket */
	}
}
