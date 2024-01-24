#ifndef DATAFUNCTIONS_H
#define DATAFUNCTIONS_H

/*includes*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "structures.h"

int dataHandling(char str[]);
int validatePORT(char str[], int* i, char PORT[]);
int validateIP(char str[], int* i, char IP[]);
nod* newFirstNod(int id, char IP[], char PORT[]);
eob * makeExternOrBackup(int id, char IP[], char PORT[]);
nod* readDjoin(int join, char str[], int* net, char IP[], char PORT[]);
int djoin(nod* no, int net);
int messageCommand(char buf[], int* i);
int message(char buf[], int* id, char IP[], char PORT[], int i);
intr* newIntrNeighbour(int id, char IP[], char PORT[]);
int messageUDP(char buf[], nod * no, int net);
void eliminateNod(nod** no, struct addrinfo * resultados);
int createContent(cont** conteudo, char buffer[]);
int writeContent(char buffer[], int i);
void deleteContent(cont** conteudo, char buffer[]);
void readContents(cont* conteudo);
void freeName(cont** conteudo);
int readGetDest(char buf[], int* id);
int searchContent(cont *conteudo, char buffer[]);
int searchExpTable(nod* no, int dest_id, int neigh_id, int source_id, char content[], int neigh_fd);
int createExpTableEntry(nod* no, int dest_id, int neigh_id, int fd);
int proccessCommand(char buf[], int* first_id, int* second_id, int i);
int fillDestExpTable(nod* no, int source_id, int neigh_id, int fd);
int contentExpTable(nod* no, int source_id, int dest_id, int neigh_id, char content[], int cmd, int neigh_fd);
void showRouting(nod* no);
void clearRouting(nod *no);
int freeExpTableEntries(nod * no, int node_left, int neigh);
int proccessWITHDRAW(int i, char buf[]);
void printMenu(void);
void readFragmentedMessage(char buf[], int fd, int bytes_read);

#endif