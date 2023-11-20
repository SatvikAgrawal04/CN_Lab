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
	connect_sock.sin_port = htons(5000);
	connect_sock.sin_family = AF_INET;
	// int ret = inet_aton("192.168.255.141", &connect_sock.sin_addr);
	connect_sock.sin_addr.s_addr = INADDR_ANY;

	int conn_ret = connect(sockfd, (const struct sockaddr *)&connect_sock, sizeof(connect_sock));

	char send_buf[100] = {"Files present in the current directory"};
	// send_buf = "Files present in the current directory"
	send(sockfd, send_buf, strlen(send_buf), 0);
	// if (!strcmp(send_buf, "bye"))
	//	break;

	FILE *fd = fopen("cl_res.txt", "w");
	char recv_buf[100];
	int ret = 0;
	while (1)
	{
		ret += recv(sockfd, recv_buf, sizeof(recv_buf), 0);
		if (ret >= 100)
		{
			break;
		}

		printf("message: %s\n", recv_buf);

		fwrite(recv_buf, 1, 10, fd);
	}

	close(sockfd);
}
