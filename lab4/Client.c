#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Error in creating socket");
		exit(0);
	}

	struct sockaddr_in connect_sock;
	connect_sock.sin_port = htons(6900);
	connect_sock.sin_family = AF_INET;
	int ret = inet_aton("192.168.101.141", &connect_sock.sin_addr);

	int conn_ret = connect(sockfd, (const struct sockaddr *)&connect_sock, sizeof(connect_sock));

	while (1)
	{
		char send_buf[100];
		printf("Enter message: \n");
		scanf("\n%s", send_buf);
		send(sockfd, send_buf, strlen(send_buf), 0);
		if (!strcmp(send_buf, "bye"))
			break;

		char recv_buf[100];
		int ret = recv(sockfd, recv_buf, sizeof(recv_buf), 0);
		recv_buf[ret] = '\0';
		printf("message: %s\n", recv_buf);
	}

	close(sockfd);
}