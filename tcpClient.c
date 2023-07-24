#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    int client_socket;
    struct sockaddr_in server_address;

    // Create UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr));

    int numbers[3];
    printf("Enter 3 numbers: ");
    for (int i = 0; i < 3; i++)
    {
        scanf("%d", &numbers[i]);
    }

    // Send numbers to the server
    sendto(client_socket, (char *)numbers, sizeof(numbers), 0, (struct sockaddr *)&server_address, sizeof(server_address));

    // Receive the largest number from the server
    int largest_number;
    int server_address_len = sizeof(server_address);
    recvfrom(client_socket, (char *)&largest_number, sizeof(largest_number), 0, (struct sockaddr *)&server_address, &server_address_len);

    printf("The largest number received from the server: %d\n", largest_number);

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
