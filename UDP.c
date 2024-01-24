/*defines*/
#define buffer_size 2699
#define _DEFAULT_SOURCE

/*includes*/

//normal includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//includes para sockets
#include <sys/types.h>
#include <sys/socket.h>

//include para structs
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

/*declaração de variaveis globais e estruturas*/

#include "structures.h"
#include "dataFunctions.h"
#include "UDP.h"

int UDPClientInit(struct addrinfo **resultados){

	//declaracoes
	int fd;
	struct addrinfo hints;

	// criar socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1){
		return -1;
	}

	// limpar o hints
	memset(&hints, 0, sizeof(hints));

	// preencher o hints	
	hints.ai_family = AF_INET;          // IPv4
	hints.ai_socktype = SOCK_DGRAM;     // UDP socket

	// obter informação sobre o endereço do server ao qual nos queremos ligar	
	if(getaddrinfo("tejo.tecnico.ulisboa.pt", "59000", &hints, resultados)!= 0){
		printf("Erro a conectar com o servidor de nós.\n");
		return -1;
	}

	return fd;

}

ssize_t UDP_send(int fd, struct addrinfo * resultados,char buffer[]){

	ssize_t n;

	n = sendto(fd, buffer, strlen(buffer), 0, resultados->ai_addr, resultados->ai_addrlen);

	if(n == -1){
	
		printf("Erro a enviar a mensagem.\n");
		return -1;
	}

	// printf("Enviou a mensagem.\n");
	return n;
}

ssize_t UDP_receive(int fd, struct addrinfo * resultados,char buffer[]){
	ssize_t n;
	struct sockaddr_in addr;
	socklen_t addrlen;

	addrlen = sizeof(addr);
	n = recvfrom(fd, buffer, buffer_size, 0, (struct sockaddr*)&addr, &addrlen);
	//printf("%ld\n", n);

	if(n == -1){
		printf("Erro a receber a informação do servidor de nós.\n");
		return -1;
	}
	return n;
}

/*void UDP_close(int fd, struct addrinfo * resultados){
	close(fd);
	freeaddrinfo(resultados);
}*/