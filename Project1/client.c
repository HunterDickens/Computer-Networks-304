#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 6789
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[200];

    // Check if the path is provided as a command-line argument
    char *path = "/HelloWorld.html"; // Default path
    if (argc > 1) {
        path = argv[1];
    }

    // Construct the GET request message
    sprintf(message, "GET %s HTTP/1.1\r\nHost: localhost\r\n\r\n", path);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Resolve host name
    struct hostent *server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    // Send request
    if (write(sock, message, strlen(message)) < 0) {
        perror("ERROR writing to socket");
        exit(2);
    }

    // Receive response
    if (read(sock, buffer, BUFFER_SIZE) < 0) {
        perror("ERROR reading from socket");
        exit(3);
    }

    // Print response
    printf("%s\n", buffer);

    // Close socket
    close(sock);

    return 0;
}
