#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Address.hpp"

int main(int argc,char* argv[]){


	//solito controllo errori	
	if(argc != 4){
		printf("USAGE: %s IP PORT MESSAGE\n", argv[0]);
		return -1;
	}
	//inizializzo un address
	Address server(argv[1], atoi(argv[2]));
	//prendo messaggio da linea di comando
	char* msg = argv [3];

	//inizializzo/apro il socket
	int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	//solito controllo errori
	if(socket_id == -1) errore("socket()", -2);

	/*
	Rimane uguale a quella del client in C perchè ancora non abbiamo il socketUDP. 
	L'unico parametro che cambia è il quinto, 
	che viene passato per referenza tramite un metodo del server che ritorna alla struttura sockaddr_in 
	*/
	int rc = sendto(socket_id, msg, strlen(msg)+1, 0, (struct sockaddr*) server.getAddress(), (socklen_t) sizeof(struct sockaddr_in));

	//solito controllo errori
	if(rc!= strlen(msg)+1){
		errore("sendto", -3);
	}
	printf("invia a [%s:%d] '%s'\n",server.getIp(), server.getPort(), msg);

	//struttura che serve alla recvfrom da modificare, per vedere chi ci ha scritto
	Address* mitt = new Address();
	//contenitore dove mettiamo il messaggio del mittente
	char* buffer = (char*) malloc(sizeof(char) * (MAX_STR + 1));
	//definisce la grandezza della struttura sockaddr
	int addr_len = sizeof(struct sockaddr);

	/*
	Rimane uguale a quella del client in C perchè ancora non abbiamo il socketUDP. 
	L'unico parametro che cambia è il quinto.
	*/
	rc = recvfrom(socket_id,(void*) buffer, MAX_STR, 0, (struct sockaddr*) mitt->getAddress(), (socklen_t*) &addr_len);
	if(rc <= 0){
		errore("recvform()", -4);
	}

	//mettiamo alla fine del buffer un carattere di fine stringa per avere un po' di sicurezza in più
	buffer[rc] = '\0';

	//dichiariamo una stringa contenente l'ip del mittente, recuperata dalla variabile mitt
	char* ip_mitt = strdup(mitt->getIp());
	//contiene la porta del mittente
	int port_mitt = ntohs(mitt->getPort());
	printf("ricevuto da [%s:%d] %s \n", ip_mitt, port_mitt, buffer);

	//chiudiamo/liberiamo tutto
	close(socket_id);
	free(buffer);

	return 0;
}