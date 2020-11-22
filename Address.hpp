#ifndef __ADDRESS_HPP
#define __ADDRESS_HPP

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "utilities.h"
#include <iostream>

#define MAX_STR 16384
#define PORT 8080


#define IP_LO "127.0.0.1"
#define IP_DHCP "0.0.0.0"



class Address {
private:
    struct sockaddr_in *address;
    int port;
    char *ip;
    pthread_mutex_t *mutex_id;


public:
    Address(char *ip, int port);

    Address(struct sockaddr_in *);


    Address(Address &);

    Address();

    ~Address();


    struct sockaddr_in *setIp(char *);

    struct sockaddr_in *setPort(int);

    struct sockaddr_in *setAddress(struct sockaddr_in);

    char *getIp();

    int getPort();

    sockaddr_in *getAddress();

    char *toString();

    pthread_mutex_t *getMutexInstance();

    void cleanMutex();

};


pthread_mutex_t *Address::getMutexInstance() {
    if (!this->mutex_id) {
        this->mutex_id = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
        std::cout << "creo nuovo mutex \n";
        int rc = pthread_mutex_init(this->mutex_id, NULL);
        if(rc)
            errore("creazione mutex", -12);
    }

    std::cout << "restituisco mutex \n";
    return this->mutex_id;
}

void Address::cleanMutex() {
    free(this->mutex_id);
}

Address::Address() {
    this->address = (sockaddr_in *) malloc(sizeof(sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = PORT;
    inet_aton(IP_LO, &this->address->sin_addr);
    for (int i = 0; i < 8; i++) {
        address->sin_zero[i] = 0;
    }
}

Address::Address(char *ip, int port) {
    this->address = (sockaddr_in *) malloc(sizeof(sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = port;
    inet_aton(ip, &address->sin_addr);
    for (int i = 0; i < 8; i++) {
        address->sin_zero[i] = 0;
    }
}

Address::Address(Address &mitt) {
    this->address->sin_family = AF_INET;
    this->address->sin_port = mitt.getPort();
    inet_aton(mitt.getIp(), &this->address->sin_addr);
    for (int i = 0; i < 8; i++) {
        this->address->sin_zero[i] = 0;
    }

}

Address::Address(struct sockaddr_in *sockaddr) {
    this->address = (sockaddr_in *) malloc(sizeof(sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = sockaddr->sin_port;
    address->sin_addr = sockaddr->sin_addr;
    for (int i = 0; i < 8; i++) {
        address->sin_zero[i] = 0;
    }
}

Address::~Address() {
    this->cleanMutex();
}

struct sockaddr_in *Address::setIp(char *ip) {
    pthread_mutex_lock(this->getMutexInstance());
    inet_aton(ip, &address->sin_addr);
    pthread_mutex_unlock(this->getMutexInstance());
    return address;
}

struct sockaddr_in *Address::setPort(int port) {
    address->sin_port = htons(port);

    return address;
}

struct sockaddr_in *Address::setAddress(struct sockaddr_in sockaddr) {
    address->sin_family = AF_INET;
    address->sin_port = sockaddr.sin_port;
    address->sin_addr = sockaddr.sin_addr;
    for (int i = 0; i < 8; i++) {
        address->sin_zero[i] = 0;
    }
    return address;

}

char *Address::getIp() {
    pthread_mutex_lock(this->getMutexInstance());
    char *ret = inet_ntoa(address->sin_addr);
    pthread_mutex_unlock(this->getMutexInstance());

    return ret;
}

int Address::getPort() {
    return address->sin_port;
}

sockaddr_in *Address::getAddress() {
    return address;
}

char *Address::toString() {
    char buffer[MAX_STR + 1];

    return strdup(buffer);
}


#endif