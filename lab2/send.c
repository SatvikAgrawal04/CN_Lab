#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

int main()
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		printf("Error in creating socket");
		exit(0);
	}

	struct sockaddr_in bind_sock;
	bind_sock.sin_port = 5000;
	bind_sock.sin_family = AF_INET;
	bind_sock.sin_addr.s_addr = INADDR_ANY;

	struct sockaddr_in send_sock;
	send_sock.sin_port = 4000;
	send_sock.sin_family = AF_INET;
	send_sock.sin_addr.s_addr = INADDR_ANY;

	struct sockaddr_in recv_sock;
	int len = sizeof(recv_sock);

	int bind_ret = bind(sockfd, (const struct sockaddr *)&bind_sock, sizeof(bind_sock));
	if(bind_ret == -1)
	{
		printf("Error in bind socket");
		exit(0);
	}
		char bye[] = "bye";


	while(1)
	{
	char send_buf[100];
	printf("Enter message: ");
	scanf("%s", send_buf);

	

	int send_ret = sendto(sockfd, send_buf, strlen(send_buf), 0, (const struct sockaddr *)&send_sock, sizeof(send_sock));
	if(send_ret == -1)
	{
		printf("Error in sendto");
		exit(0);
	}
	if(!strcmp(send_buf, bye))
	{
		printf("disconnected\n");
		break;
	}

	

	char recv_buf[100];
	int rec_ret = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&recv_sock, &len);
	if(rec_ret == -1)
	{
		printf("Error in recvfrom");
		exit(0);
	}
	else{
		printf("message recieved from sender: %s\n", recv_buf);
	}
	if(!strcmp(recv_buf, bye))
	{
		printf("Sender wants to disconnect\n");
		break;
	}

}
	
	close(sockfd);

	return 0;
}