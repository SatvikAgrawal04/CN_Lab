#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error in creating socket\n");
        exit(0);
    }

    struct sockaddr_in bind_sock;
    bind_sock.sin_port = 8000;
    bind_sock.sin_family = AF_INET;
    bind_sock.sin_addr.s_addr = INADDR_ANY;

    int bind_ret = bind(sockfd, (const struct sockaddr *)&bind_sock, sizeof(bind_sock));
    if (bind_ret == -1)
    {
        printf("Error in bind");
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
    int clinetfds[1024];

    while (1)
    {
        readfdset = watchfdset;
        int ret = select(nfds, &readfdset, NULL, NULL, NULL);
        if (ret > 0)
        {
            if (FD_ISSET(sockfd, &readfdset))
            {
                struct sockaddr_in accept_sock;
                int len = sizeof(accept_sock);
                int datafd = accept(sockfd, (struct sockaddr *)&accept_sock, &len);
                if (datafd == -1)
                {
                    printf("Error in accept");
                    exit(0);
                }
                FD_SET(datafd, &watchfdset);
                clinetfds[i] = datafd;
                i++;
                if (datafd >= nfds)
                {
                    nfds = datafd + 1;
                }
            }
            for (int j = 0; j <= i; j++)
            {
                if (FD_ISSET(clinetfds[j], &readfdset))
                {
                    char buff[1024];
                    int ret = recv(clinetfds[j], &buff, sizeof(buff), 0);
                    buff[ret] = '\0';
                    printf("message from client: %s\n", buff);
                    if (!strcmp(buff, "bye") || (ret == 0))
                    {
                        printf("Disconnecting...");
                        FD_CLR(clinetfds[j], &watchfdset);
                        close(clinetfds[j]);
                    }

                    char send_buff[1024];
                    printf("Enter a message: \n");
                    fgets(send_buff, 1024, stdin);
                    send(clinetfds[j], &send_buff, strlen(send_buff), 0);
                }
            }
        }
    }
    close(sockfd);
    return 0;
}
