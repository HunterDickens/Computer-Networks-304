#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 6789
#define BUFFER_SIZE 1024

void *handleRequest(void *socketDesc) {
    int socket = *((int*)socketDesc);
    free(socketDesc);

    char buffer[BUFFER_SIZE];
    int file;
    ssize_t bytesRead;

    read(socket, buffer, BUFFER_SIZE);
    char *filename = strtok(buffer, " ");
    filename = strtok(NULL, " ");
    if (filename) {
        if (filename[0] == '/') filename++; // Remove leading slash

        file = open(filename, O_RDONLY);
        if (file < 0) {
            char *notFoundMsg = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>\r\n";
            write(socket, notFoundMsg, strlen(notFoundMsg));
        } else {
            char *okMsg = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";
            write(socket, okMsg, strlen(okMsg));
            while ((bytesRead = read(file, buffer, BUFFER_SIZE)) > 0)
                write(socket, buffer, bytesRead);
            close(file);
        }
    }
    close(socket);
    return 0;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        int *new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handleRequest, (void*) new_sock) < 0) {
            perror("could not create thread");
            return 1;
        }
        pthread_detach(thread_id);
    }

    return 0;
}

