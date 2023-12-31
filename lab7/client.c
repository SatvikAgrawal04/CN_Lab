#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Error in creating socket");
		exit(0);
	}

	struct sockaddr_in connect_sock;
	connect_sock.sin_port = htons(8000);
	connect_sock.sin_family = AF_INET;
	connect_sock.sin_addr.s_addr = INADDR_ANY;

	int conn_ret = connect(sockfd, (const struct sockaddr *)&connect_sock, sizeof(connect_sock));

	while (1)
	{
		char send_buf[100];
		printf("Enter message: \n");
		fgets(send_buf, 100, stdin);
		send(sockfd, send_buf, strlen(send_buf), 0);
		if (!strcmp(send_buf, "bye"))
			break;

		char recv_buf[100];
		int ret = recv(sockfd, recv_buf, sizeof(recv_buf), 0);
		recv_buf[ret] = '\0';
		printf("message from: %s\n", recv_buf);
	}

	close(sockfd);
}
