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
#include	<stdint.h>
typedef struct gtp_header_t
{
	uint8_t flag;
	uint8_t type;
	uint16_t length;
	uint32_t teid;
}__attribute__((packed)) gtp_header;

int main(int argc, char **argv)
{
	int sockfd,udpfd,n,sendbufferlen;
	
	struct sockaddr_in	cliaddr, servaddr,remoteaddr;
	char line[1500];
	void *sendbuffer;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(5202);

	bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

	for(;;)
	{
		bzero(&line, sizeof(line));
		socklen_t clilen = sizeof(cliaddr);
		n = recvfrom(sockfd, line,1500, 0, (struct sockaddr*)&cliaddr, &clilen);
		//printf("%s %d %d\n",line,strlen(line),sizeof(gtp_header));
		sendbuffer = calloc(2000,sizeof(char));
		gtp_header* gtpuheader=calloc(1,sizeof(gtp_header));
		gtpuheader->flag = 0x30;
		gtpuheader->type = 255;
		gtpuheader->length = htons(strlen(line));
		gtpuheader->teid = htonl(0);
		memcpy(sendbuffer,gtpuheader, sizeof(gtp_header));		
		memcpy(sendbuffer+sizeof(gtp_header), line, strlen(line)+1);
		
		printf("after read\n");
		udpfd = socket(AF_INET, SOCK_DGRAM, 0);
		bzero(&remoteaddr, sizeof(remoteaddr));
		remoteaddr.sin_family = AF_INET;
		remoteaddr.sin_port = htons(2152);
		inet_pton(AF_INET, "127.0.0.15", &remoteaddr.sin_addr);
		printf("%d\n",setsockopt(udpfd,SOL_SOCKET,SO_BINDTODEVICE,"enp0s9",6));
		sendto(udpfd,sendbuffer,strlen(line)+8,0,(struct sockaddr*)&remoteaddr,sizeof(remoteaddr));
		free(sendbuffer);
		free(gtpuheader);
		
		close(udpfd);
	}
}
