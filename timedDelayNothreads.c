/*
Author: ThankGod Hunyinbo
Course: CPAN226
Lab: Lab 4 - Multithreading and Network Concurrency
Description:
Multithreaded TCP server that handles multiple clients concurrently
using pthreads. Each client request is processed in its own thread.
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <pthread.h>

#define PORT 8080

// Function that handles each client connection
void* handle_client(void* arg)
{
    SOCKET client_socket = *(SOCKET*)arg;
    free(arg);

    printf("Client connected\n");

    // Simulate server work
    Sleep(5000);

    printf("Finished processing client\n");

    closesocket(client_socket);

    return NULL;
}

int main()
{
    WSADATA wsa;
    SOCKET server_socket, *client_socket;
    struct sockaddr_in server, client;
    int c;

    printf("Initializing Winsock...\n");
    WSAStartup(MAKEWORD(2,2), &wsa);

    server_socket = socket(AF_INET , SOCK_STREAM , 0 );

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server , sizeof(server));

    listen(server_socket , 5);

    printf("Server waiting for connections...\n");

    c = sizeof(struct sockaddr_in);

    while((client_socket = malloc(sizeof(SOCKET))))
    {
        *client_socket = accept(server_socket ,
            (struct sockaddr *)&client, &c);

        if (*client_socket < 0)
        {
            perror("accept failed");
            return 1;
        }

        pthread_t thread_id;

        // Create new thread for client
        if (pthread_create(&thread_id , NULL , handle_client ,
            (void*)client_socket) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        // Automatically free thread resources
        pthread_detach(thread_id);
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}
