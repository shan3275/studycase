/* Client.c
 * Copyright Mark Watson 1999. Open Source Software Lience.
 * Note: derived from NLPserver client example.
 * See www.markwatson.com/opensource/opensource.htm for all
 * of the natural language server (NLPserver) source code.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

char * host_name = "192.168.1.104"; //local host
int port = 8000;

int main(int argc, char *argv[]) {
    char buf[8192];
    char message[256];
    int socket_descriptor;
    struct sockaddr_in pin;
    struct hostent *server_host_name;

    char str[256] = "A default test string";

    if (argc < 2) {
        printf("Usage:'test \"Any test string\"\n");
        printf("We will send a default test string.\n");
    } else {
        memset(str, 0, sizeof(str));
        memcpy(str,argv[1], strlen(argv[1]));
    }

    if ((server_host_name = gethostbyname(host_name)) == 0) {
        perror("Error resolving local host\n");
        exit(EXIT_FAILURE);
    }

    bzero(&pin, sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = htonl(INADDR_ANY);
    pin.sin_addr.s_addr = ((struct in_addr *)(server_host_name->h_addr))->s_addr;
    pin.sin_port = htons(port);

    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error opening socket\n");
        exit(EXIT_FAILURE);
    }

    if (connect(socket_descriptor, (void *)&pin, sizeof(pin)) == -1) {
        perror("Error connecting to socket\n");
        exit(EXIT_FAILURE);
    }

    printf("Sending message %s to server...\n", str);

    if (send(socket_descriptor, str, strlen(str), 0) == -1) {
        perror("Error in send\n");
        exit(EXIT_FAILURE);
    }

    printf("..sent message.. wait for response...\n");

    memset(buf, 0, sizeof(buf));
    if (recv(socket_descriptor, buf, 8192, 0) == -1) {
        perror("Error in receving response from server\n");
        exit(EXIT_FAILURE);
    }

    printf("\nResponse from server:\n\n%s\n", buf);

    close(socket_descriptor);
    return 0;
}
