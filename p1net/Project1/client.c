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
    char message[BUFFER_SIZE];
    strcpy(message, "GET ");
    strcat(message, argv[1]);
    strcat(message, " HTTP/1.1\r\nHost: localhost\r\n\r\n");


    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    struct hostent *server = gethostbyname("localhost");
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    write(sock, message, strlen(message));
    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    close(sock);

    return 0;
}

