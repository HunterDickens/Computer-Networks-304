#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define PORT 6789
#define BUFFER_SIZE 1024

void sendResponse(int socket, const char *header, const char *contentType, const char *content, int contentLength) {
    char responseHeader[BUFFER_SIZE];
    int headerLength = snprintf(responseHeader, BUFFER_SIZE, "%sContent-Type: %s\r\nContent-Length: %d\r\n\r\n", header, contentType, contentLength);
    write(socket, responseHeader, headerLength);
    if (contentLength > 0) {
        write(socket, content, contentLength);
    }
}

void serveFile(int socket, const char *filename) {
    char buffer[BUFFER_SIZE];
    int file = open(filename, O_RDONLY);

    if (file < 0) {
        char *notFoundContent = "<html><body><h1>404 Not Found</h1></body></html>";
        sendResponse(socket, "HTTP/1.0 404 NOT FOUND\r\n", "text/html", notFoundContent, strlen(notFoundContent));
    } else {
        struct stat fileStat;
        fstat(file, &fileStat);
        char *okHeader = "HTTP/1.0 200 OK\r\n";
        char *contentType = "text/html"; // This should be determined based on the file extension

        sendResponse(socket, okHeader, contentType, NULL, fileStat.st_size);

        while (1) {
            ssize_t bytesRead = read(file, buffer, BUFFER_SIZE);
            if (bytesRead == 0) break;
            write(socket, buffer, bytesRead);
        }
        close(file);
    }
}
void *handleRequest(void *arg){
int new_socket = *((int *)arg);
    char buffer[BUFFER_SIZE] = {0};

    read(new_socket, buffer, BUFFER_SIZE);

    char *method = strtok(buffer, " ");
    char *filename = strtok(NULL, " ");
    if (method && filename && strcmp(method, "GET") == 0) {
        if (filename[0] == '/') filename++;
        serveFile(new_socket, filename);
    } else {
        char *badRequestContent = "<html><body><h1>400 Bad Request</h1></body></html>";
        sendResponse(new_socket, "HTTP/1.0 400 BAD REQUEST\r\n", "text/html", badRequestContent, strlen(badRequestContent));
    }

    close(new_socket);
    pthread_exit(NULL);
}

int main() {
    int server_fd;
    int *new_socket = malloc(sizeof(int));
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        int temp = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        *new_socket = temp;
        
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        pthread_t tid; // Thread ID
        if (pthread_create(&tid, NULL, handleRequest, new_socket) != 0) {
            perror("Thread creation failed");
            close(*new_socket);
            free(new_socket); // Free memory if thread creation fails
            continue;
        }
        pthread_detach(tid); // Detach the thread to avoid memory leaks
    }

    close(server_fd);
    return 0;
}