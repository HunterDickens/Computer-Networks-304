#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 6789
#define BUFFER_SIZE 1024

void serveFile(int socket, const char *filename) {
    char buffer[BUFFER_SIZE];
    int file = open(filename, O_RDONLY);
    if (file < 0) {
        char *notFoundMsg = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>\r\n";
        write(socket, notFoundMsg, strlen(notFoundMsg));
    } else {
        char *okMsg = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";
        write(socket, okMsg, strlen(okMsg));
        while (1) {
            ssize_t bytesRead = read(file, buffer, BUFFER_SIZE);
            if (bytesRead == 0) break;
            write(socket, buffer, bytesRead);
        }
        close(file);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        read(new_socket, buffer, BUFFER_SIZE);
        
        // Simple parsing to get the filename from the request
        char *filename = strtok(buffer, " ");
        filename = strtok(NULL, " ");
        if (filename) {
            if (filename[0] == '/') filename++; // Remove leading slash
            serveFile(new_socket, filename);
        }

        close(new_socket);
    }

    return 0;
}

