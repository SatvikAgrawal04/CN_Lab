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
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Error in creating socket");
		exit(0);
	}

	struct sockaddr_in connect_sock;
	connect_sock.sin_port = 6000;
	connect_sock.sin_family = AF_INET;
	connect_sock.sin_addr.s_addr = INADDR_ANY;

	int conn_ret = connect(sockfd, (const struct sockaddr *)&connect_sock, sizeof(connect_sock));
	if (conn_ret == -1)
	{
		print("Error in connect\n");
		exit(0);
	}

	while (1)
	{
		char send_buf[100];
		printf("Enter message: \n");
		scanf("\n%s", send_buf);
		send(sockfd, send_buf, strlen(send_buf), 0);
		if (!strcmp(send_buf, "bye"))
		{
			printf("Disconnected\n");
			break;
		}

		char recv_buf[100];
		int ret = recv(sockfd, recv_buf, sizeof(recv_buf), 0);
		recv_buf[ret] = '\0';
		printf("message: %s\n", recv_buf);
		if (!strcmp(recv_buf, "bye"))
		{
			printf("Server wants to Disconnect\n");
			break;
		}
	}

	close(sockfd);
}

/*
socket()
connect()
send()
recv()
*/