#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Address.hpp"
#include <iostream>
#include "signal.h"

using namespace std;

#define PROMPT "$>"
#define EXIT_CMD "quit"
#define IP_DHCP "0.0.0.0"


typedef struct {
    char *msg;
    int sock_id;
} client;


void *listener(void *demon);

int main(int argc, char **argv) {


    if (argc != 3) {
        printf("USAGE: %s PORT MESSAGE\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);
    char *msg = argv[2];
    Address *mySelf = new Address(IP_LO, port);

    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_id == -1)errore("socket()", -2);


    int rc = bind(socket_id, (struct sockaddr *) mySelf->getAddress(), (socklen_t)
    sizeof(struct sockaddr));
    //controllo errori
    if (rc != 0)errore("bind()", -3);

    client parametri = {msg, socket_id};

    pthread_t demon_id;
    rc = pthread_create(&demon_id, NULL, listener, (void *) &parametri);
    if (rc != 0)errore("pthread_create()", -4);


    printf("%s", PROMPT);
    char *command = inputStr();
    printf("\n");
    while (strcmp(command, EXIT_CMD)) {
        free(command);
        printf("%s", PROMPT);
        command = inputStr();
        printf("\n");
    }
    pthread_kill(demon_id, NULL);


    free(command);
    close(socket_id);

    return 0;
}


void *listener(void *demon) {

    client *param = static_cast<client *>(demon);

    char buffer[MAX_STR + 1];

    Address *mitt = new Address();

    int addr_len = sizeof(struct sockaddr);


    /*---*/
    for (;;) {

        int rc = recvfrom(param->sock_id, buffer, MAX_STR, 0, (struct sockaddr *) mitt->getAddress(),
                          (socklen_t * ) & addr_len);

        if (rc <= 0) errore("recvfrom()", -5);

        buffer[rc] = '\0';

        printf("%s ricevuto da [%s:%d] '%s'\n", PROMPT, mitt->getIp(), mitt->getPort(), buffer);

        rc = sendto(param->sock_id, param->msg, strlen(param->msg) + 1, 0, (struct sockaddr *) mitt->getAddress(),
                    (socklen_t)
        sizeof(struct sockaddr));

        if (rc != strlen(param->msg) + 1) errore("sendto()", -6);
        printf("%s invia a [%s:%d] '%s'\n", PROMPT, mitt->getIp(), mitt->getPort(), param->msg);
    }
    /*---*/


    return NULL;
}