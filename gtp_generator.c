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
	int sockfd,udpfd,n;
	
	struct sockaddr_in servaddr,remoteaddr;
	char line[1500];
	void *sendbuffer,*recvbuffer;
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(5202);

	bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

	for(;;)
	{
		recvbuffer = calloc(2000,sizeof(char));
		bzero(&line, sizeof(line));
		printf("before receive:\n");
		n = recvfrom(sockfd, recvbuffer,2000, 0, NULL, NULL);
		printf("after receive: %d\n",n);
		sendbuffer = calloc(2000,sizeof(char));
		gtp_header* gtpuheader=calloc(1,sizeof(gtp_header));
		gtpuheader->flag = 0x30;
		gtpuheader->type = 255;
		gtpuheader->length = htons(n+sizeof(gtp_header));
		gtpuheader->teid = htonl(0);
		memcpy(sendbuffer,gtpuheader, sizeof(gtp_header));		
		memcpy(sendbuffer+sizeof(gtp_header), recvbuffer, n);
			
		printf("after read and adding header:\n");
		udpfd = socket(AF_INET, SOCK_DGRAM, 0);
		bzero(&remoteaddr, sizeof(remoteaddr));
		remoteaddr.sin_family = AF_INET;
		remoteaddr.sin_port = htons(2152);
		inet_pton(AF_INET, "10.0.2.6", &remoteaddr.sin_addr);
		printf("%d\n",setsockopt(udpfd,SOL_SOCKET,SO_BINDTODEVICE,"enp0s9",6));
		sendto(udpfd,sendbuffer,n+sizeof(gtp_header),0,(struct sockaddr*)&remoteaddr,sizeof(remoteaddr));
		free(sendbuffer);
		free(gtpuheader);
		free(recvbuffer);
		close(udpfd);
	}
}
