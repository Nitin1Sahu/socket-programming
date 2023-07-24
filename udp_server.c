#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_PORT 8888

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    int server_socket;
    struct sockaddr_in server_address, client_address;

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        printf("Error binding: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    int numbers[3];

    while (1)
    {
        // Receive the numbers from the client
        int client_address_len = sizeof(client_address);
        recvfrom(server_socket, (char *)numbers, sizeof(numbers), 0, (struct sockaddr *)&client_address, &client_address_len);

        // Find the largest number among the received numbers
        int largest_number = numbers[0];
        for (int i = 1; i < 3; i++)
        {
            if (numbers[i] > largest_number)
            {
                largest_number = numbers[i];
            }
        }

        // Send the largest number back to the client
        sendto(server_socket, (char *)&largest_number, sizeof(largest_number), 0, (struct sockaddr *)&client_address, sizeof(client_address));
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
