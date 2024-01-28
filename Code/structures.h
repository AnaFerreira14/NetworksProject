#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

/* A estrutura intr guarda os conteúdos 
do nó interno e um apontador para outra
estrutura intr. Esta estrutura serve para
guardar os nós não externos*/
typedef struct internal{
    int id;
    char IP[20];
	char PORT[20];

	int fd_socket_tcp_server;
    
    struct internal *next_intr;
}intr;

/* A estrutura cont guarda um apontador para outra
estrutura content e um vetor de caracteres */
typedef struct con{
    char CONTENT[100];

    struct con *next_cont;
}cont;

/* A estrutura exptable guarda o ID do vizinho
que permite chegar ao destino cujo ID é id_dest*/
typedef struct et{
    int id_dest;
    int id_neigh;
    int fd_socket_tcp_neigh;

    struct et *next_exp_tab;
}exptable;

typedef struct extorbck{
    int id;
    char IP[20];
	char PORT[20];

	int fd_socket_tcp_server;
}eob;

/* A estrutura nod guarda o ID do próprio nó, o ID do vizinho
externo, o ID do vizinho de recuperação e o conjunto com os 
IDs dos vizinhos não externos*/
typedef struct node{
    int id;
    char IP[20];
	char PORT[20];

    int fd_socket_udp;
	int fd_socket_tcp_server;
    
    struct extorbck *ext;
    struct extorbck *backup;
    struct internal *inter;
    
    struct con *content;
    struct et *expedition_table;
}nod;

#endif