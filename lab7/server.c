#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
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

	struct sockaddr_in bind_sock;
	bind_sock.sin_port = htons(8000);
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

	int nfds = sockfd + 1;
	fd_set readfdset, watchfdset;
	FD_ZERO(&readfdset);
	FD_ZERO(&watchfdset);
	FD_SET(sockfd, &watchfdset);
	int i = 0;
	int clientfd[1024];
	while (1)
	{
		readfdset = watchfdset;
		int ret = select(nfds, &readfdset, NULL, NULL, NULL);
		if (ret > 0)
		{
			if (FD_ISSET(sockfd, &readfdset))
			{
				struct sockaddr_in accept_sock;
				int accept_sock_size = sizeof(accept_sock);
				int datafd = accept(sockfd, (struct sockaddr *)&accept_sock, &accept_sock_size);
				FD_SET(datafd, &watchfdset);
				clientfd[i] = datafd;
				i++;
				if (datafd >= nfds)
				{
					nfds = datafd + 1;
				}
			}
			for (int j = 0; j <= i; j++)
			{
				if (FD_ISSET(clientfd[j], &readfdset))
				{
					char recv_buf[100];
					int ret = recv(clientfd[j], recv_buf, sizeof(recv_buf), 0);
					recv_buf[ret] = '\0';
					printf("message from client %d: %s\n", j, recv_buf);
					if (ret == 0)
					{
						FD_CLR(clientfd[j], &watchfdset);
						close(clientfd[j]);
					}

					char send_buf[100];
					printf("Enter message: \n");
					fgets(send_buf, 100, stdin);
					send(clientfd[j], send_buf, strlen(send_buf), 0);
				}
			}
		}
	}
}
