#ifndef TCP_H
#define TCP_H

/*includes*/
#include "structures.h"

int TCP_Server_Init(nod *no);
int TCP_Client_Init(char IP[], char PORT[]);
int TCP_Client_Connect(int server_fd);
int TCP_send(int fd, char *mensagem);
int TCP_receive(int fd , char *in_str, int buffersize);

#endif