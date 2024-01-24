#ifndef UDP_H
#define UDP_H

/*includes*/
#include "structures.h"

int UDPClientInit(struct addrinfo **resultados);
ssize_t UDP_send(int fd, struct addrinfo * resultados,char buffer[]);
ssize_t UDP_receive(int fd, struct addrinfo * resultados,char buffer[]);

#endif