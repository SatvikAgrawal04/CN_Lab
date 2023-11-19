#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

void handle_client(int client_socket)
{
    char buffer[1024];

    // Receive data from client
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received from client: %s\n", buffer);

    // Send ASCII values back to client
    char ascii_values[1024];
    int i;
    for (i = 0; i < strlen(buffer); i++)
    {
        sprintf(ascii_values + strlen(ascii_values), "%d ", buffer[i]);
    }
    send(client_socket, ascii_values, strlen(ascii_values), 0);

    // Receive converted characters from client
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Converted characters from client: %s\n", buffer);

    // Close the connection
    close(client_socket);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        // Accept a connection
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        printf("Got connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle client communication
        handle_client(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
