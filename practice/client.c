#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in conn_sock;
    conn_sock.sin_port = 8000;
    conn_sock.sin_family = AF_INET;
    conn_sock.sin_addr.s_addr = INADDR_ANY;

    int conn_ret = connect(sockfd, (const struct sockaddr *)&conn_sock, sizeof(conn_sock));
    if (conn_ret == -1)
    {
        printf("Error in connect");
        exit(0);
    }

    while (1)
    {
        char send_buf[1024];
        printf("Enter a message: \n");
        fgets(send_buf, 1024, stdin);
        send(sockfd, &send_buf, sizeof(send_buf), 0);

        char recv_buff[1024];
        int ret = recv(sockfd, &recv_buff, strlen(recv_buff), 0);
        recv_buff[ret] = '\0';
        printf("Message from server: %s", recv_buff);
        if (!strcmp(recv_buff, "bye"))
        {
            printf("server wants to disconnect...\n");
            break;
        }
    }
    close(sockfd);

    return 0;
}
