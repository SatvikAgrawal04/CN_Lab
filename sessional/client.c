#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Send a sequence of characters to the server
    char message[] = "Hello, Server!";
    send(client_socket, message, strlen(message), 0);
    printf("Sent to server: %s\n", message);

    // Receive ASCII values from the server
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received ASCII values from server: %s\n", buffer);

    // Convert ASCII values back to characters
    // Note: This assumes that the ASCII values are space-separated
    char *token = strtok(buffer, " ");
    printf("Converted characters: ");
    while (token != NULL)
    {
        printf("%c", (char)atoi(token));
        token = strtok(NULL, " ");
    }
    printf("\n");

    // Send the converted characters back to the server
    send(client_socket, buffer, strlen(buffer), 0);

    // Close the client socket
    close(client_socket);

    return 0;
}
