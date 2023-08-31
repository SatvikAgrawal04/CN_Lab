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

	struct sockaddr_in bind_sock;
	bind_sock.sin_port = 6000;
	bind_sock.sin_family = AF_INET;
	bind_sock.sin_addr.s_addr = INADDR_ANY;

	int bind_ret = bind(sockfd, (const struct sockaddr *)&bind_sock, sizeof(bind_sock));
	if (bind_ret == -1)
	{
		printf("Error in bind socket");
		exit(0);
	}

	int listen_ret = listen(sockfd, 5);
	if (listen_ret == -1)
	{
		printf("Error in listen");
		exit(0);
	}

	struct sockaddr_in accept_sock;
	int accept_sock_size = sizeof(accept_sock);
	int accept_fd = accept(sockfd, (struct sockaddr *)&accept_sock, &accept_sock_size);
	if (accept_fd == -1)
	{
		printf("Error in accept");
		exit(0);
	}

	while (1)
	{

		char recv_buf[100];
		int ret = recv(accept_fd, recv_buf, sizeof(recv_buf), 0);
		recv_buf[ret] = '\0';
		printf("message: %s\n", recv_buf);
		if (!strcmp(recv_buf, "bye"))
		{
			printf("Client wants to Disconnect\n");
			break;
		}

		char send_buf[100];
		printf("Enter message: \n");
		scanf("\n%s", send_buf);
		send(accept_fd, send_buf, strlen(send_buf), 0);
		if (!strcmp(send_buf, "bye"))
		{
			printf("Disconnected\n");
			break;
		}
	}

	close(sockfd);
	return 0;
}

/*
socket()
bind()
listen()
accept()
recv()
send()
close()
*/