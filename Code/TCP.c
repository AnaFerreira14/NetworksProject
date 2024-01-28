/*defines*/
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
#include "TCP.h"


/********************************
 * Função: 
 *		TCP_Server_Init()
 * Descrição geral da função:
 * 		Inicia um cliente TCP na propria maquina
 * Entradas:
 *		nod no, o proprio nó
 * Saida:
 * 		Inteiro que indica a situacao de saida da funcao. 
 * 		NO entanto esta funcao tb modifica o struct Node
 *		correspondente ao no proprio
 ********************************/
int TCP_Server_Init(nod *no){

	//variaveis locais
	int fd;
	struct addrinfo hints, *resultados, *res;
	
	//criar socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1){
		printf("Erro a abrir a socket TCP\n");
		return -1;
	}

	// Limpar o hints
	memset(&hints, 0, sizeof(hints));
	
	//preencher o hints	
	hints.ai_family = AF_INET;          // IPv4
	hints.ai_socktype = SOCK_STREAM;     // TDP socket
	hints.ai_flags = AI_PASSIVE;		// For wildcard IP address
	
	//ver addr na propria maquina
	if(getaddrinfo(NULL, no->PORT, &hints, &resultados)!= 0){
		fprintf(stderr, "Erro em getaddrinfo\n");
		return -1;
	}

	//dar bind da socket ao addr retornado em resultados
	for (res = resultados; res != NULL; res = res->ai_next) {
		if (bind(fd, res->ai_addr, res->ai_addrlen) == 0){
        	break;/* Success */
		}                            
	}

	if (res == NULL) {               /* No address succeeded */
		close(fd);
		fprintf(stderr, "Erro em bind()\n");
		return -1;
	}

	//gravar a socket no struct do no
	no->fd_socket_tcp_server = fd;
	
	//printf("Em TCP_Server_Init, o valor de fd_socket_tcp_server fica: %d\n\n", no->fd_socket_tcp_server);
	
	//colocar a socket em modo listen
	if(listen(fd, 69)==-1){
		printf("Erro em listen\n");
		exit(1);
	}

	freeaddrinfo(resultados);           
 		
return 0;
}

/********************************
 * Função: 
 *		TCP_Client_Init()
 * Descrição geral da função:
 * 		Inicia um cliente TCP
 * Entradas:
 *		Node do nó ao qual nos queremos ligar
 * Saida:
 * 		Inteiro que indica a situacao de saida da funcao
 *      em caso de insuccesso e o file descritor da
 *      socket criada em caso de successo. 
 ********************************/

int TCP_Client_Init(char IP[], char PORT[]){ 
//para cada ligação teremos de criar um cliente novo, uma vez q este fica afeto a uma socket exclusiva.

	//variaveis locais
	struct addrinfo hints, *resultados, *res;
	int fd;
	
	//criação da socket
	fd = socket(AF_INET,SOCK_STREAM, 0);
	
	//limpar o hints
	memset(&hints,0,sizeof(hints));
	
	//preencher o hints
	hints.ai_family=AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;     // TCP socket
	
	//ver addr do server ao qual nos queremos ligar	
	if(getaddrinfo(IP, PORT, &hints, &resultados)!= 0){
		fprintf(stderr, "Erro em getaddrinfo\n");
		return -1;
	}
	
	//Para o caso de existirem varios addr, tentar todos, até conseguir dar connect
	for (res = resultados; res != NULL; res = res->ai_next) {

		if (connect(fd, res->ai_addr, res->ai_addrlen) == 0){
			break;
		}                 
	}
	freeaddrinfo(resultados);           //no longer needed

	if (res == NULL) {               //No address succeeded
		fprintf(stderr, "Erro em connect()\n");
		return -1;
	}

	return fd;
}

/********************************
 * Função: 
 *		TCP_Client_Connect()
 * Descrição geral da função:
 * 		Conecta um cliente TCP a um servidor TCP
 * Entradas:
 *		Node do nó ao qual nos queremos ligar
 * Saida:
 * 		Inteiro que indica a situacao de saida da funcao
 *      em caso de insuccesso e o file descritor da
 *      socket criada em caso de successo.
 ********************************/
int TCP_Client_Connect(int server_fd){
    int client_fd = accept(server_fd, NULL, NULL);

    if(client_fd == -1){
        printf("Não foi possível conectar com o servidor.\n");
    }
	
    return client_fd;
}

/********************************
 * Função: 
 *		TCP_send()
 * Descrição geral da função:
 * 		Envia uma mensagem dada, por TCP 
 * Entradas:
 *		Node para o qual quero enviar a mensagem
 * Saida:
 * 		Situacao de saida da funcao.
 ********************************/
int TCP_send(int fd, char *mensagem){

	int s;
	
	//printf("Chegou a TCP_send\n\n");
	s = write(fd, mensagem, strlen(mensagem));
	if(s == -1){
		/*error*/ 
		fprintf(stderr, "Erro em write do TCP_send()\n");
		return -1;
	}

return 0;
}


/********************************
 * Função: 
 *		TCP_receive()
 * Descrição geral da função:
 * 		recebe uma mensagem vinda por TCP
 * Entradas:
 *		.
 * Saida:
 * 		Mensagem recebida
 ********************************/
int TCP_receive(int fd , char *in_str, int buffersize){

	//variaveis locais
	int s;
	
	s = read(fd, in_str, buffersize);
	if(s == -1){
		/*error*/ 
		fprintf(stderr, "Erro em read de TCP_receive()\n");
		return -1;
	}


return s;
}

