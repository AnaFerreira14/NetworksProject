// Defines
#define MSG_SIZE 40
#define C_SIZE 128

// Includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TCP.h"
#include "dataFunctions.h"
#include "structures.h"
#include "UDP.h"

/*******************************
 * Descrição geral da funcao:
 * Trata da string recebida via teclado. Retorna um inteiro que representa a validade do comando.
 * 		.
 * Entradas: string
 *		.
 * Saida: inteiro
 * 		.
 *******************************/

int dataHandling(char str[]){
    if(str[0]== 'j' && str[1]== 'o' && str[2]== 'i' && str[3]== 'n'){
        return 1;
    }
    else if(str[0]== 'd' && str[1]== 'j' && str[2]== 'o' && str[3]== 'i' && str[4]== 'n'){
        return 2;
    }
    else if(str[0]== 'l' && str[1]== 'e' && str[2]== 'a' && str[3]== 'v' && str[4]== 'e'){
        return 3;
    }
    else if(str[0]== 'e' && str[1]== 'x' && str[2]== 'i' && str[3]== 't'){
        return 4;
    }
    else if((str[0] == 's' && str[1]== 'h' && str[2]== 'o' && str[3]== 'w' && str[4]== ' ' && str[5]== 't' && str[6]== 'o' && str[7]== 'p' && str[8]== 'o' && str[9]== 'l' && str[10]== 'o' && str[11]== 'g' && str[12]== 'y') || (str[0] == 's' && str[1]== 't')){
        return 5;
    }
    else if(str[0]== 'c' && str[1]== 'r' && str[2]== 'e' && str[3]== 'a' && str[4]== 't' && str[5]== 'e' && str[6]== ' '){
        return 6;
    }
    else if(str[0]== 'd' && str[1]== 'e' && str[2]== 'l' && str[3]== 'e' && str[4]== 't' && str[5]== 'e' && str[6]== ' '){
        return 7;
    }
    else if((str[0] == 's' && str[1]== 'h' && str[2]== 'o' && str[3]== 'w' && str[4]== ' ' && str[5]== 'n' && str[6]== 'a' && str[7]== 'm' && str[8]== 'e' && str[9]== 's') || (str[0] == 's' && str[1]== 'n')){
        return 8;
    }
    else if(str[0] == 'g' && str[1]== 'e' && str[2]== 't' && str[3]== ' '){
        return 9;
    }
    else if((str[0] == 's' && str[1]== 'h' && str[2]== 'o' && str[3]== 'w' && str[4]== ' ' && str[5]== 'r' && str[6]== 'o' && str[7]== 'u' && str[8]== 't' && str[9]== 'i' && str[10]== 'n' && str[11]== 'g') || (str[0] == 's' && str[1]== 'r')){
        return 10;
    }
    else if((str[0] == 'c' && str[1]== 'l' && str[2]== 'e' && str[3]== 'a' && str[4]== 'r' && str[5]== ' ' && str[6]== 'r' && str[7]== 'o' && str[8]== 'u' && str[9]== 't' && str[10]== 'i' && str[11]== 'n' && str[12]== 'g') || (str[0] == 'c' && str[1]== 'r')){
        return 11;
    }
    else{
		return -1; //Comando invalido
    }
}

/*******************************
 * Descrição geral da função:
 * 		Verifica a validade de um porto. Retorna
 *      0 se o porto for válido e -1 em caso contrario
 * 		
 * Entradas: net id
 *		.
 * Saida: inteiro
 * 		.
 *******************************/
int validatePORT(char str[], int* i, char PORT[]){
    int nrdigit = 0, j = -1, num = 0;

    while(str[*i] != ' ' && str[*i] != '\0' && str[*i] != '\n'){
        if(!isdigit(str[*i])){
            return -1;
        }
        ++nrdigit;
        if(nrdigit > 5){return -1;}
        ++j;
        PORT[j] = str[*i];
        ++(*i);
    }

    if(nrdigit == 5){
        num = (str[*(i)-1] - '0') + (str[*(i)-2] - '0')*10 + (str[*(i)-3] - '0')*100 
        + (str[*(i)-4] - '0')*1000 + (str[*(i)-5] - '0')*10000;
        if(num > 65535){return -1;}
    }

    if(nrdigit == 4){
        num = (str[*(i)-1] - '0') + (str[*(i)-2] - '0')*10 + (str[*(i)-3] - '0')*100 
        + (str[*(i)-4] - '0')*1000;
        if(num < 1024){return -1;}
    }

    *i -= 1;
    PORT[++j] = '\0';
    return 0;
}


/*******************************
 * Descrição geral da função:
 * 		Verifica a validade de um endereço IP. Retorna
 *      0 se o endereço for válido e -1 em caso contrario
 * 		
 * Entradas: str i IP
 *		.
 * Saida: inteiro
 * 		.
 *******************************/
int validateIP(char str[], int* i, char IP[]){
    int dots = 0, num = 0, j = -1, nrdigit = 0;

    while(str[*i] != ' ' && str[*i] != '\0'){

        if(isdigit(str[*i])){
            num = 1;
            ++nrdigit;
        }
        else if(str[*i] == '.' && num == 1){
            if(nrdigit == 3){
                num = (str[*(i)-1] - '0') + (str[*(i)-2] - '0')*10 + (str[*(i)-3] - '0')*100;
                //printf("%d\n",num);
                if(num > 255){return -1;}
            }
            nrdigit = 0;
            ++dots;
            if(dots > 3){return -1;}
            num = 0;
        }
        else{return -1;}
        ++j;
        IP[j] = str[*i];
        ++(*i);
    }
    if(nrdigit == 3){
        nrdigit = (str[*(i)-1] - '0') + (str[*(i)-2] - '0')*10 + (str[*(i)-3] - '0')*100;
        //printf("%d\n",nrdigit);
        if(nrdigit > 255){return -1;}
    }
    if(dots != 3 || num != 1){return -1;}
    *i -= 1;
    IP[++j] = '\0';
    return 0;
}

/*******************************
 * Descrição geral da função:
 * 		Cria um novo no
 * 		
 * Entradas: id IP PORT
 *		.
 * Saida: inteiro
 * 		.
 *******************************/

nod* newFirstNod(int id, char IP[], char PORT[]){
    nod * first_node =(nod*)malloc(sizeof(nod));
    if(first_node == NULL){
        printf("Erro a criar o nó.\n");
        return NULL;
    }
    first_node->id = id;
    first_node->ext = NULL;
    first_node->backup = NULL;
    first_node->inter = NULL;
    first_node->content = NULL;
    first_node->expedition_table = NULL;
    first_node->fd_socket_tcp_server = -1;
    first_node->fd_socket_udp = -1;
    memset(first_node->IP, '\0', 20);
    memset(first_node->PORT, '\0', 20);
    strcpy(first_node->IP, IP);
	strcpy(first_node->PORT, PORT);
    // printf("Nó criado com sucesso.\n");
    return first_node;
    
}

/*******************************
 * Descrição geral da função:
 * 		Cria um novo no externo ou de backup
 * 		
 * Entradas: id IP PORT
 *		.
 * Saida: eob
 * 		.
 *******************************/

eob * makeExternOrBackup(int id, char IP[], char PORT[]){
    eob * new_node = (eob*)malloc(sizeof(eob));
    if(new_node == NULL){
        printf("Erro a criar o nó.\n");
        return NULL;
    }
    new_node->id = id;
    new_node->fd_socket_tcp_server = -1;
    memset(new_node->IP, '\0', 20);
    memset(new_node->PORT, '\0', 20);
    strcpy(new_node->IP, IP);
	strcpy(new_node->PORT, PORT);

    return new_node;
}

/*******************************
 * Descrição geral da função:
 * 		Le as palavras provenientes do teclado, 
 *      para a funcao djoin ou join, e modifica
 *      o seu tipo de dados de acordo com o seu 
 *      uso posterior.
 * 		
 * Entradas: net id
 *		.
 * Saida: ponteiro para o no
 * 		.
 *******************************/

nod* readDjoin(int join, char str[], int* net, char IP[], char PORT[]){
    // Declaracoes
    int i = 0, id = -1, bootid = -1;
    char bootIP[20], bootTCP[20];
    nod* no = NULL;

    // Verifica se o comando esta completo
    if(strlen(str)<15 && join == 0){
		printf("Comando incompleto.\nUso de djoin: djoin net id bootid bootIP bootTCP\n");
		return NULL;
	}

    if(strlen(str)<8 && join == 1){
		printf("Comando incompleto.\nUso de join: join net id\n");
		return NULL;
	}

    if(join == 1){i = 4;}
    else{i = 5;}

    // Espaco entre o comando djoin e net
    if(str[i] != ' '){return NULL;}

    // Verifica o numero de digitos da rede
    if(str[i+2] == ' '){
        if(!isdigit(str[i+1])){return NULL;}

        *net = str[i+1] - '0';
        i = i+3;
    }
    else if(str[i+3] == ' '){
        if(!isdigit(str[i+1]) || !isdigit(str[i+2])){return NULL;}

        *net = (str[i+2] - '0') + (str[i+1] - '0')*10;
        i = i+4;
    }
    else if(str[i+4] == ' '){
        if(!isdigit(str[i+1]) || !isdigit(str[i+2]) || !isdigit(str[i+3])){return NULL;}

        *net = (str[i+3] - '0') + (str[i+2] - '0')*10 + (str[i+1] - '0')*100;
        i = i+5;
    }
    else{return NULL;}

    //printf("net: %d\n", *net);

    // Verifica o numero de digitos de id
    // Se a variavel join estiver a 0, ou seja, estamos a ler o 
    // comando djoin, o numero de digitos de bootid é verificado

    while(bootid == -1){

        if(join == 0){
            if(str[i+1] == ' '){
                if(!isdigit(str[i])){return NULL;}

                if(id == -1){
                    id = (str[i] - '0');
                }
                else{
                    bootid = (str[i] - '0');
                }
                i += 2;
            }

            else if(str[i+2] == ' '){
                if(!isdigit(str[i]) || !isdigit(str[i+1])){return NULL;}

                if(id == -1){
                    id = (str[i] - '0')*10 + (str[i+1] - '0');
                }
                else{
                    bootid = (str[i] - '0')*10 + (str[i+1] - '0');
                }
                i += 3;
            }

            else{return NULL;}

            //if(bootid ==-1){printf("id: %d\n", id);}
            //else{printf("bootid: %d\n", bootid);}
        }

        if(join == 1){
            if(str[i+1] == '\n'){
                if(!isdigit(str[i])){return NULL;}

                id = (str[i] - '0');
            }

            else if(str[i+2] == '\n'){
                if(!isdigit(str[i]) || !isdigit(str[i+1])){return NULL;}

                id = (str[i] - '0')*10 + (str[i+1] - '0');
            }

            else{return NULL;}

            // Criar o nó para join

            if((no = newFirstNod(id, IP, PORT)) == NULL){return NULL;}

            return no;
        }
    }

    // Verifica se bootIP é um endereco valido

    if(validateIP(str, &i, bootIP) == -1){return NULL;}

    //printf("IP: %s\n", bootIP);

    if(str[i+1] != ' '){
        //printf("Erro na leitura.\n");
        return NULL;
    }
    i += 2;

    // Verifica se bootTCP é um porto valido

    if(validatePORT(str, &i, bootTCP) == -1){return NULL;}

    //printf("PORT: %s\n", bootTCP);

    if(id == bootid){
        // Comparar o IP com o bootIP
        if(strcmp(bootIP, IP) != 0){
            return NULL;
        }
        // Comparar o PORT com o bootTCP
        if(strcmp(bootTCP, PORT) != 0){
            return NULL;
        }
    }

    // Criar o nó para o djoin

    if((no = newFirstNod(id, IP, PORT)) == NULL){return NULL;}

    // Se o nó não for único na rede, criar o seu vizinho externo

    if(id != bootid){
        if((no->ext = makeExternOrBackup(bootid, bootIP, bootTCP)) == NULL){
            free(no);
            return NULL;
        }
    }

    return no;

}

/*******************************
 * Descrição geral da função:
 * 		Adiciona um nó com identificador id à rede net que é
 *      único na rede. É passado à aplicação o identificador 
 *      e o contacto de um nó da rede, através dos parâmetros
 *      bootid, bootIP e bootTCP, ao qual o nó se deverá ligar 
 *      sem interrogar o servidor de nós. Se bootid for igual 
 *      a id então cria-se a rede só com um nó.
 * 		
 * Entradas: no net
 *		.
 * Saida: int
 * 		.
 *******************************/

int djoin(nod* no, int net){
    int fd_client = -1; // Socket de conexão do nó com identificador id com o nó bootid

    // Inicialização do servidor TCP do nó em questão
    if(TCP_Server_Init(no) == 0){
        printf("A socket TCP server do nó foi criada com successo.\n");
    }
    else{
        printf("Ocorreu um erro na criação da socket TCP server do nó.\n");
        return -1;
    }

    // O identificador id é igual a bootid, por isso o nó é
    // o primeiro a entrar na rede
    if(no->ext == NULL){
        no->backup = makeExternOrBackup(no->id, no->IP, no->PORT);
        if(no->backup == NULL){
            printf("Não foi possível criar o vizinho de recuperação.\n");
            return -1;
        }
        no->ext = makeExternOrBackup(no->id, no->IP, no->PORT);
        if(no->ext == NULL){
            free(no->backup);
            printf("Não foi possível criar o vizinho externo.\n");
            return -1;
        }
        // printf("Nó âncora:\n");
        // printf("id: %d\nIP: %s\nSocket TCP: %d\nRede: %d\n",
        //    no->id,no->IP,no->fd_socket_tcp_server,net);

        printf("O primeiro nó da rede foi criado.\nO ID deste nó é %d\n\n", no->id);
    }

    else{
        // Função que estabelece uma comunicação entre o nó
        // com identificador id e o nó com identificador bootid
        if((fd_client = TCP_Client_Init(no->ext->IP, no->ext->PORT)) == -1){
            printf("Não foi possível comunicar com o nó cujo id é %d.\n", no->ext->id);
            free(no->ext);
            return -1;
        }
        no->ext->fd_socket_tcp_server = fd_client;

        // printf("Nó:\n");
        // printf("id: %d\nIP: %s\nSocket TCP: %d\nRede: %d\n",
        //    no->id,no->IP,no->fd_socket_tcp_server,net);
        //printf("Nó externo:\n");
        //printf("id: %d\nIP: %s\nSocket TCP: %d\nRede: %d\n",
        //    no->ext->id,no->ext->IP,no->ext->fd_socket_tcp_server,net);

        // Criar a mensagem
        char buffer[MSG_SIZE];
        int i = 0;
        int bytes_read = snprintf(buffer, sizeof(buffer), "NEW %d %s %s\n", no->id, no->IP, no->PORT);
        if(bytes_read < 0){return -1;}
        //buffer[bytes_read] = '\0';
        for(i = bytes_read; i<MSG_SIZE; i++){
            buffer[i] = '\0';
        }

        // Enviar o comando NEW
        if(TCP_send(fd_client, buffer)==-1){
            printf("Não foi possível enviar a mensagem NEW ao nó externo.\n");
            return -1;
        }

        //bytes_read = write(1, buffer, MSG_SIZE);
        //printf("\n");
        printf("O nó com ID %d foi criado.\nO vizinho externo do nó criado tem ID %d.\n\n", no->id, no->ext->id);
    }

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Lê o comando de uma mensagem recebida através 
 *      de comunicação TCP. Esta funcao devolve o
 *      comando que foi lido.
 * 		
 * Entradas: buf id IP PORT
 *		.
 * Saida: int
 * 		.
 *******************************/
int messageCommand(char buf[], int* i){
    int cmd = 0;

    // Verifica o comando

    if(buf[0] == 'N' && buf[1] == 'E' && buf[2] == 'W'){
        *i = 4;
        cmd = 1;
    }

    else if(buf[0] == 'E' && buf[1] == 'X' && buf[2] == 'T' && buf[3] == 'E' && buf[4] == 'R' && buf[5] == 'N'){
        *i = 7;
        cmd = 2;
    }

    else if(buf[0] == 'Q' && buf[1] == 'U' && buf[2] == 'E' && buf[3] == 'R' && buf[4] == 'Y'){
        *i = 6;
        cmd = 3;
    }

    else if(buf[0] == 'C' && buf[1] == 'O' && buf[2] == 'N' && buf[3] == 'T' && buf[4] == 'E' && buf[5] == 'N' && buf[6] == 'T'){
        *i = 8;
        cmd = 4;
    }

    else if(buf[0] == 'N' && buf[1] == 'O' && buf[2] == 'C' && buf[3] == 'O' && buf[4] == 'N' && buf[5] == 'T' && buf[6] == 'E' && buf[7] == 'N' && buf[8] == 'T'){
        *i = 10;
        cmd = 5;
    }

    else if(buf[0] == 'W' && buf[1] == 'I' && buf[2] == 'T' && buf[3] == 'H' && buf[4] == 'D' && buf[5] == 'R' && buf[6] == 'A' && buf[7] == 'W'){
        *i = 9;
        cmd = 6;
    }

    else{
        *i = 0;
        return -1;
    }

    return cmd;
}

/*******************************
 * Descrição geral da função:
 * 		Lê o comando de uma mensagem recebida através 
 *      de comunicação TCP. Esta funcao devolve o
 *      comando que foi lido.
 * 		
 * Entradas: buf id IP PORT
 *		.
 * Saida: int
 * 		.
 *******************************/

int message(char buf[], int* id, char IP[], char PORT[], int i){

    if(buf[i-1] != ' '){return -1;}

    // Verifica se o nó é válido

    if(buf[i+1] == ' '){
        if(!isdigit(buf[i])){return -1;}

        *id = (buf[i] - '0');

        i += 2;
    }

    else if(buf[i+2] == ' '){
        if(!isdigit(buf[i]) || !isdigit(buf[i+1])){return -1;}

        *id = (buf[i] - '0')*10 + (buf[i+1] - '0');
            
        i += 3;
    }

    else{return -1;}

    // Verifica se o IP é válido

    // Verifica se bootIP é um endereco valido

    if(validateIP(buf, &i, IP) == -1){return -1;}

    if(buf[i+1] != ' '){return -1;}
    i += 2;

    // Verifica se bootTCP é um porto valido

    if(validatePORT(buf, &i, PORT) == -1){return -1;}

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Cria um novo vizinho interno
 * 		
 * Entradas: id IP PORT
 *		.
 * Saida: intr
 * 		.
 *******************************/

intr* newIntrNeighbour(int id, char IP[], char PORT[]){
    intr * first_node =(intr*)malloc(sizeof(intr));
    if(first_node == NULL){
        printf("Erro a criar o nó.\n");
        return NULL;
    }
    first_node->id = id;
    first_node->fd_socket_tcp_server = -1;
    strcpy(first_node->IP, IP);
	strcpy(first_node->PORT, PORT);
    printf("Nó interno criado com sucesso.\n");
    return first_node;
    
}


/*******************************
 * Descrição geral da função:
 * 		Lê o comando de uma mensagem recebida através 
 *      de comunicação UDP. Esta funcao devolve o
 *      comando que foi lido.
 * 		
 * Entradas: buf id IP PORT
 *		.
 * Saida: int
 * 		.
 *******************************/

int messageUDP(char buf[], nod * no, int net){
    int i = 0, cmd = 0, id = 0, j = 1, nrnods = 0;
    char IP[20], PORT[20];

    // Verifica o comando

    if(buf[0] == 'N' && buf[1] == 'O' && buf[2] == 'D' && buf[3] == 'E' && buf[4] == 'S' && buf[5] == 'L' && buf[6] == 'I' && buf[7] == 'S' && buf[8] == 'T'){
        cmd = 1;
        i = 13;
        while(buf[i] != '\0'){
            if(buf[i] == ' ' || buf[i] == '\n'){
                ++i;
            }
            else{
            
                // j = 1 significa que estamos a ler o parâmetro id
                if(j == 1){
                    if(buf[i + 1] == ' '){
                        if(!isdigit(buf[i])){return -1;}
                        id = (buf[i] - '0');
                        ++i;
                    }
                    else if(buf[i + 2] == ' '){
                        if(!isdigit(buf[i]) || !isdigit(buf[i+1])){return -1;}
                        id = (buf[i] - '0')*10 + (buf[i+1] - '0');
                        i+=2;
                    }
                    else{return -1;}

                    // Verificar que o nó não está na lista de nós do servidor de nós
                    if(id == no->id){
                        printf("O nó que se está a tentar registar já existe na lista de nós.\n");
                        return -1;
                    }
                }

                // j = 2 significa que estamos a ler o parâmetro IP
                if(j == 2){
                    memset(IP, '\0', 20);
                    if(validateIP(buf, &i, IP) == -1){return -1;}
                    IP[strlen(IP)] = '\0';
                    ++i;
                }

                // j = 3 significa que estamos a ler o parâmetro PORT
                if(j == 3){
                    memset(PORT, '\0', 20);
                    if(validatePORT(buf, &i, PORT) == -1){return -1;}
                    PORT[strlen(PORT)] = '\0';
                    ++i;
                }

                if(j<3){++j;}
                else{
                    ++nrnods;
                    j = 1;
                }
            }
        }

        if(nrnods >= 100){
            printf("A rede já possui o número máximo de nós. Não é possível adicionar mais nenhum nó.\n");
            return -1;
        }

        if(nrnods == 0){ // não há nenhum nó na rede
            printf("Primeiro nó na rede.\n");
        }

        else{ // há nós na rede
            // Criar nó externo
            if((no->ext = makeExternOrBackup(id, IP, PORT)) == NULL){
                return -1;
            }
            // printf("%s\n", IP);
            // printf("%s\n", PORT);
        }

        // Chamar a função djoin
        if(djoin(no, net) == -1){
            printf("Erro no join.\n");
            return -1;
        }

    }

    else if(buf[0] == 'O' && buf[1] == 'K' && buf[2] == 'R' && buf[3] == 'E' && buf[4] == 'G'){
        printf("O nó foi registado no servidor de nós.\n");
        printf("Join concluído com sucesso.\n\n");
        cmd = 2;
    }

    else if(buf[0] == 'O' && buf[1] == 'K' && buf[2] == 'U' && buf[3] == 'N' && buf[4] == 'R' && buf[5] == 'E' && buf[6] == 'G'){
        printf("O nó foi retirado do servidor de nós.\n");
        printf("Leave concluído com sucesso.\n\n");
        cmd = 3;
    }

    else{return -1;}

    return cmd;

}

/*******************************
 * Descrição geral da função:
 * 		Elimina um no
 * 		
 * Entradas: nod** addrinfo*
 *		.
 * Saida: 
 * 		.
 *******************************/
void eliminateNod(nod** no, struct addrinfo * resultados){
    intr * aux = NULL, * intern = NULL;
    cont * ct = NULL, * aux2 = NULL;
    exptable * expt1 = NULL, * expt2 = NULL;

    if((*no) != NULL){

        if((*no)->ext != NULL){
            if(((*no)->ext->fd_socket_tcp_server != -1) && ((*no)->ext->id != (*no)->id)){
                close((*no)->ext->fd_socket_tcp_server);
            }
            free((*no)->ext);
            (*no)->ext = NULL;
        }
    
        if((*no)->backup != NULL){
            if(((*no)->backup->fd_socket_tcp_server != -1) && ((*no)->backup->id != (*no)->id)){
                close((*no)->backup->fd_socket_tcp_server);
            }
            free((*no)->backup);
            (*no)->backup = NULL;
        }

        intern = (*no)->inter;
        while(intern != NULL){
            aux = intern->next_intr;
            if(intern->fd_socket_tcp_server != -1){
                close(intern->fd_socket_tcp_server);
            }
            free(intern);
            intern = aux;
        }
        (*no)->inter = NULL;

        if((*no)->fd_socket_tcp_server != -1){
            close((*no)->fd_socket_tcp_server);
        }
        if((*no)->fd_socket_udp != -1){
            freeaddrinfo(resultados);
            close((*no)->fd_socket_udp);
        }

        ct = (*no)->content;
        while(ct != NULL){
            aux2 = ct->next_cont;
            free(ct);
            ct = aux2;
        }
        (*no)->content = NULL;

        expt1 = (*no)->expedition_table;
        while(expt1 != NULL){
            expt2 = expt1->next_exp_tab;
            free(expt1);
            expt1 = expt2;
        }
        (*no)->expedition_table = NULL;

        free((*no));
        (*no) = NULL;
    }
}

/*******************************
 * Descrição geral da função:
 * 		Cria um novo conteudo
 * 		
 * Entradas: conteudo buffer
 *		.
 * Saida: inteiro
 * 		.
 *******************************/

int createContent(cont** conteudo, char buffer[]){

    cont* aux;

    // Cria o conteúdo

    cont *new_name = (cont *)calloc(1, sizeof(cont));
    if(new_name == NULL){
        return -1;
    }

    // Copia o vetor de caracteres para o parâmetro CONTENT

    strcpy(new_name->CONTENT, buffer);

    // Introduzir o conteúdo no início da lista de conteúdos

    aux = *conteudo;
    *conteudo = new_name;
    (*conteudo)->next_cont = aux;

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Escreve o conteúdo no vetor de caracteres buffer
 * 		
 * Entradas: buffer i
 *		.
 * Saida: inteiro
 * 		.
 *******************************/

int writeContent(char buffer[], int i){
    char buf[128];
    int nr_char = 0;

    strcpy(buf, buffer);

    memset(buffer,'\0', 128);

    while(nr_char < 100 && buf[i] != '\n' && buf[i] != '\0'){
        buffer[nr_char] = buf[i];
        ++i;
        ++nr_char;
    }

    if(buffer[0] == '\0'){
        return -1;
    }

    return 0;
}


/*******************************
 * Descrição geral da função:
 * 		Elimina um conteudo
 * 		
 * Entradas: conteudo buffer
 *		.
 * Saida: 
 * 		.
 *******************************/
void deleteContent(cont**conteudo, char buffer[]){
    cont *aux, *aux2;
    int eliminate = 0;

    aux = *conteudo;
    aux2 = NULL;

    while(aux != NULL){
        if(strcmp(aux->CONTENT, buffer) == 0){
            if(aux2 == NULL){
                *conteudo = (*conteudo)->next_cont;
                free(aux);
            }
            else{
                aux2->next_cont = aux->next_cont;
                free(aux);
            }
            eliminate = 1;
            break;
        }
        aux2 = aux;
        aux = aux->next_cont;
    }

    if(eliminate == 0){
        if(aux2 == NULL){
            printf("\nNão existem conteúdos para eliminar.\n\n");
        }
        else{
            printf("\nO conteúdo não foi encontrado.\n\n");
        }
    }
    else{
        printf("\nO conteúdo foi eliminado.\n\n");
    }

    return;
}

/*******************************
 * Descrição geral da função:
 * 		Le os conteudos de um no
 * 		
 * Entradas: cont*
 *		.
 * Saida: void
 * 		.
 *******************************/
void readContents(cont* conteudo){
    cont* aux = NULL;

    if(conteudo == NULL){
        printf("\nNão existem conteúdos.\n\n");
        return;
    }

    printf("\nConteúdos:\n\n");

    aux = conteudo;
    while(aux != NULL){
        printf("%s\n", aux->CONTENT);
        aux = aux->next_cont;
    }

    return;
}


/*******************************
 * Descrição geral da função:
 * 		Liberta os conteudos de uma
 *      estrutura cont*
 * 		
 * Entradas: cont*
 *		.
 * Saida: void
 * 		.
 *******************************/
void freeName(cont** conteudo){
    cont* aux;

    while((*conteudo) != NULL){
        aux = *conteudo;
        *conteudo = (*conteudo)->next_cont;
        free(aux);
    }

    return;
}

/*******************************
 * Descrição geral da função:
 * 		Le o id de destino da quando o utilizador
 *      introduz o comando get dest name. Retorna
 *      o id dest em caso de sucesso e -1 em caso
 *      contrário.
 * 		
 * Entradas: char* int*
 *		.
 * Saida: int
 * 		.
 *******************************/
int readGetDest(char buf[], int* id){

    int i = 4;

    if(buf[i+1] == ' '){
        if(!isdigit(buf[i])){return -1;}

        *id = (buf[i] - '0');

        i += 2;
    }

    else if(buf[i+2] == ' '){
        if(!isdigit(buf[i]) || !isdigit(buf[i+1])){return -1;}

        *id = (buf[i] - '0')*10 + (buf[i+1] - '0');
            
        i += 3;
    }

    else{
        return -1;
    }

    return i;

}

/*******************************
 * Descrição geral da função:
 * 		Procura por um conteúdo igual ao vetor
 *      de caracteres buffer. Se o conteúdo
 *      for encontrado retorna 0. Caso contrário,
 *      retorna -1.
 * 		
 * Entradas: cont* char*
 *		.
 * Saida: int
 * 		.
 *******************************/
int searchContent(cont *conteudo, char buffer[]){
    cont *aux;
    int str_found = 0;

    aux = conteudo;

    while(aux != NULL){
        if(strcmp(aux->CONTENT, buffer) == 0){
            str_found = 1;
            break;
        }
        aux = aux->next_cont;
    }

    if(str_found == 0){
        return -1;
    }
    return 0;
}

/*******************************
 * Descrição geral da função:
 * 		Esta função é responsável por enviar o
 *      comando QUERY. Se o nó tiver uma entrada
 *      na tabela de expedição em que o parâmetro
 *      destino é igual ao id do nó de destino
 *      há duas possibilidades. Se o parâmetro
 *      vizinho da tabela de expedição for igual
 *      ao id do nó por onde se recebeu a mensagem,
 *      a mensagem é descartada. Caso contrário, a
 *      mensagem é enviada ao vizinho pertencente
 *      à entrada da tabela de expedição que se
 *      está a analisar. Se não for encontrada
 *      nenhuma entrada com o id do nó de destino,
 *      a mensagem é enviada a todos os vizinhos
 *      do nó. Além disso, a função verifica se 
 *      já existe alguma entrada cujo parâmetro 
 *      destino é igual ao id do nó fonte. Se não 
 *      existir o nó cria uma entrada em que o
 *      parâmetro destino é o id do nó fonte e o 
 *      parâmetro vizinho é o id do nó por onde 
 *      recebeu a mensagem. Esta função retorna -1 
 *      em caso de insucesso.
 * 		
 * Entradas: nod* int int int char* int
 *		.
 * Saida: int
 * 		.
 *******************************/
int searchExpTable(nod* no, int dest_id, int neigh_id, int source_id, char content[], int neigh_fd){

    exptable *aux = no->expedition_table;
    char buffer[C_SIZE];
    int bytes_read = 0, entry_found = 0, already_there = 0;

    memset(buffer,'\0', C_SIZE);

    if(no->id == source_id){

        // Se o nó originou a mensagem então não vai
        // ser introduzido na tabela de expedição

        already_there = 1;
    }

    while((aux != NULL) && (!entry_found || !already_there)){
        if(aux->id_dest == dest_id && entry_found == 0){
            if(aux->id_neigh == neigh_id){

                // Descartar a mensagem

                printf("Mensagem descartada.\n");

                entry_found = 1;
            }
            else{

                // Enviar a mensagem ao nó vizinho

                // Criar a mensagem
                bytes_read = snprintf(buffer, sizeof(buffer), "QUERY %d %d %s\n", dest_id, source_id, content);
                if(bytes_read < 0){return -1;}
                buffer[bytes_read] = '\0';

                // Enviar o comando QUERY
                if(TCP_send(aux->fd_socket_tcp_neigh, buffer)==-1){return -1;}

                //printf("%s\n", buffer);

                entry_found = 1;
            }
        }
        if(aux->id_dest == source_id && already_there == 0){
            // Verifica se o nó onde originou a mensagem já se encontra na tabela como destino

            already_there = 1;
        }
        aux = aux->next_exp_tab;
    }

    if(already_there == 0){
        // Criar entrada na tabela de expedição para o nó onde originou a mensagem

        if(createExpTableEntry(no, source_id, neigh_id, neigh_fd)==-1){return -1;}

    }

    if(entry_found == 1){
        return 0;
    }

    // Se a função chegar até aqui, enviar o comando QUERY a todos os vizinhos

    // Criar a mensagem
    bytes_read = snprintf(buffer, sizeof(buffer), "QUERY %d %d %s\n", dest_id, source_id, content);
    if(bytes_read < 0){return -1;}
    buffer[bytes_read] = '\0';

    //printf("%s\n", buffer);

    if(no->ext->fd_socket_tcp_server != -1){
        if(no->ext->id != neigh_id){
            // Enviar o comando QUERY
            if(TCP_send(no->ext->fd_socket_tcp_server, buffer)==-1){return -1;}
        }
    }

    intr *intern = no->inter;
    while(intern != NULL){

        if(intern->id != neigh_id){
            // Enviar o comando QUERY
            if(TCP_send(intern->fd_socket_tcp_server, buffer)==-1){return -1;}
        }

        intern = intern->next_intr;
    }

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Cria uma entrada na tabela de expedição.
 *      Se a alocação da estrutura exptable for
 *      feita com sucesso, a função retorna 0.
 *      Caso contrário, retorna -1.
 * 		
 * Entradas: nod* int int int
 *		.
 * Saida: int
 * 		.
 *******************************/
int createExpTableEntry(nod* no, int dest_id, int neigh_id, int fd){

    exptable * aux2 = (exptable*)malloc(sizeof(exptable));

    if(aux2 == NULL){
        printf("Não foi possível criar uma nova entrada na tabela de expedição.\n");
        return -1;
    }

    //printf("\nNó de destino: %d\n\n", dest_id);
    //printf("\nNó vizinho: %d\n\n", neigh_id);
    //printf("\nSocket TCP: %d\n\n", fd);

    aux2->id_dest = dest_id;
    aux2->id_neigh = neigh_id;
    aux2->fd_socket_tcp_neigh = fd;

    aux2->next_exp_tab = no->expedition_table;
    no->expedition_table = aux2;

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Esta função analisa a mensagem QUERY,
 *      CONTENT e NOCONTENT. A função recebe
 *      o vetor de caracteres com a mensagem
 *      recebida e retira o 1º id(do destino),
 *      o 2º id(da origem) e por último copia
 *      o conteúdo recebido para o buf. O 
 *      argumento i serve para identificar a
 *      posição no vetor buf onde começa o
 *      1º parâmetro da mensagem. Se algum
 *      dos parâmetros da mensagem não estiver
 *      bem escrito a função retorna -1.
 * 		
 * Entradas: char* int* int* int
 *		.
 * Saida: int
 * 		.
 *******************************/
int proccessCommand(char buf[], int* first_id, int* second_id, int i){

    if(buf[i-1] != ' '){return -1;}

    // Conseguir o primeiro ID

    if(buf[i+1] == ' '){
        if(!isdigit(buf[i])){return -1;}

        *first_id = (buf[i] - '0');

        i += 2;
    }

    else if(buf[i+2] == ' '){
        if(!isdigit(buf[i]) || !isdigit(buf[i+1])){return -1;}

        *first_id = (buf[i] - '0')*10 + (buf[i+1] - '0');
            
        i += 3;
    }

    else{return -1;}

    // Conseguir o segundo ID

    if(buf[i+1] == ' '){
        if(!isdigit(buf[i])){return -1;}

        *second_id = (buf[i] - '0');

        i += 2;
    }

    else if(buf[i+2] == ' '){
        if(!isdigit(buf[i]) || !isdigit(buf[i+1])){return -1;}

        *second_id = (buf[i] - '0')*10 + (buf[i+1] - '0');
            
        i += 3;
    }

    else{return -1;}

    // Conseguir a mensagem

    if(writeContent(buf, i) == -1){
        printf("A mensagem não possui nenhum conteúdo.\n");
        return -1;
    }

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Esta função é usada no caso em
 *      que o nó é o destino. Neste caso,
 *      a função afere se existe alguma
 *      entrada na tabela de expedição em
 *      que o parâmetro destino é igual ao
 *      id do nó fonte. Se não existir, a
 *      entrada é adicionada à tabela. Se
 *      houver um problema na alocação
 *      da entrada, a função retorna -1.
 * 		
 * Entradas: nod* int int int
 *		.
 * Saida: int
 * 		.
 *******************************/
int fillDestExpTable(nod* no, int source_id, int neigh_id, int fd){

    exptable* aux = no->expedition_table;

    while(aux != NULL){
        if(aux->id_dest == source_id){
            return 0;
        }
        aux = aux->next_exp_tab;
    }

    if(createExpTableEntry(no, source_id, neigh_id, fd) == -1){
        return -1;
    }

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Esta função procura pela entrada da tabela de
 *      expedição cujo parâmetro destino é igual ao
 *      id do nó de destino. Quando encontra esta
 *      entrada o nó manda a mensagem CONTENT ou 
 *      NOCONTENT ao parâmetro vizinho associado
 *      à entrada em questão. O argumento cmd define
 *      qual é a mensagem. Se cmd for igual a 4 a
 *      mensagem é do tipo CONTENT. Caso contrário,
 *      a mensagem é do tipo NOCONTENT. Além disso,
 *      a função verifica se já existe alguma entrada
 *      cujo parâmetro destino é igual ao id do nó fonte. 
 *      Se não existir o nó cria uma entrada em que o
 *      parâmetro destino é o id do nó fonte e o 
 *      parâmetro vizinho é o id do nó por onde recebeu
 *      a mensagem.
 * 		
 * Entradas: nod* int int int char* int int
 *		.
 * Saida: int
 * 		.
 *******************************/
int contentExpTable(nod* no, int source_id, int dest_id, int neigh_id, char content[], int cmd, int neigh_fd){

    exptable *aux = no->expedition_table;
    char buffer[C_SIZE];
    int bytes_read = 0, entry_found = 0, already_there = 0;

    memset(buffer,'\0', C_SIZE);

    while(aux != NULL && (!entry_found || !already_there)){

        if(dest_id == aux->id_dest && entry_found == 0){

            // Criar a mensagem

            if(cmd == 4){
                bytes_read = snprintf(buffer, sizeof(buffer), "CONTENT %d %d %s\n", dest_id, source_id, content);
            }
            else{
                bytes_read = snprintf(buffer, sizeof(buffer), "NOCONTENT %d %d %s\n", dest_id, source_id, content);
            }
            
            if(bytes_read < 0){return -1;}
            buffer[bytes_read] = '\0';

            // printf("%s\n", buffer);

            // Enviar a mensagem

            if(TCP_send(aux->fd_socket_tcp_neigh, buffer)==-1){return -1;}

            entry_found = 1;
        }

        if(aux->id_dest == source_id && already_there == 0){

            // Verifica se o nó onde originou a mensagem já se encontra na tabela como destino

            already_there = 1;
        }

        aux = aux->next_exp_tab;
    }

    if(already_there == 0){

        // Criar entrada na tabela em que o nó destino é o nó que originou a mensagem

        if(createExpTableEntry(no, source_id, neigh_id, neigh_fd)==-1){return -1;}
    }

    return 0;
}

/*******************************
 * Descrição geral da função:
 * 		Mostra as entradas da tabela de expedição.
 * 		
 * Entradas: nod*
 *		.
 * Saida: void
 * 		.
 *******************************/
void showRouting(nod* no){

    exptable* aux = no->expedition_table;

    if(aux == NULL){
        printf("\nA tabela de expedição está vazia.\n\n");
        return;
    }

    printf("\nTabela de Expedição\n");

    while(aux != NULL){
        printf("\n");
        printf("Destino: %d\n", aux->id_dest);
        printf("Vizinho: %d\n", aux->id_neigh);
        printf("Socket: %d\n", aux->fd_socket_tcp_neigh);
        aux = aux->next_exp_tab;
    }

    printf("\n");

    return;

}

/*******************************
 * Descrição geral da função:
 * 		Elimina a tabela de expedição.
 * 		
 * Entradas: nod*
 *		.
 * Saida: void
 * 		.
 *******************************/
void clearRouting(nod *no){

    exptable * aux = no->expedition_table, * aux2 = NULL;

    while(aux != NULL){
        aux2 = aux->next_exp_tab;
        free(aux);
        aux = aux2;
    }

    no->expedition_table = NULL;

    printf("\nA tabela de expedição foi limpa.\n\n");

}

/*******************************
 * Descrição geral da função:
 * 		Esta função elimina todas as entradas da
 *      tabela de expedição em que o parâmetro
 *      destino ou vizinho é igual ao id do nó
 *      que foi retirado da rede. No caso em que
 *      é o parâmetro destino que é igual ao nó
 *      retirado, o nó envia a mensagem WITHDRAW
 *      a todos os seus vizinhos.
 * 		
 * Entradas: nod* int int
 *		.
 * Saida: int
 * 		.
 *******************************/
int freeExpTableEntries(nod * no, int node_left, int neigh){

    exptable * aux = no->expedition_table, * aux2 = NULL, * save = NULL;
    int bytes_read = 0;
    char buffer[C_SIZE];

    while(aux != NULL){
        if((aux->id_neigh == node_left) || (aux->id_dest == node_left)){

            // Enviar a mensagem a todos os vizinhos

            if(aux->id_dest == node_left){

                // Criar a mensagem
                bytes_read = snprintf(buffer, sizeof(buffer), "WITHDRAW %d\n", node_left);
                if(bytes_read < 0){return -1;}
                buffer[bytes_read] = '\0';

                //printf("%s\n", buffer);

                if(no->ext->fd_socket_tcp_server != -1 && no->ext->id != neigh){
                    if(TCP_send(no->ext->fd_socket_tcp_server, buffer)==-1){return -1;}
                }

                intr * intern = no->inter;

                while(intern != NULL){
                    if(intern->fd_socket_tcp_server != -1 && intern->id != neigh){
                        if(TCP_send(intern->fd_socket_tcp_server, buffer)==-1){return -1;}
                    }
                    intern = intern->next_intr;
                }

            }

            // Eliminar a entrada na tabela de expedição

            if(aux2 == NULL){
                save = aux->next_exp_tab;
                free(aux);
                no->expedition_table = save;
                aux = no->expedition_table;
            }
            else{
                aux2->next_exp_tab = aux->next_exp_tab;
                free(aux);
                aux = aux2->next_exp_tab;
            }

            if(node_left != neigh){
                break;
            }

        }

        else{
            aux2 = aux;
            aux = aux->next_exp_tab;
        }

    }

    return 0;

}

/*******************************
 * Descrição geral da função:
 * 		Esta função analisa o vetor de caracteres buf
 *      e extrai o id do nó que foi retirado da rede.
 *      O parâmetro i é a primeira posição do id do
 *      nó retirado que se segue ao comando WITHDRAW
 *      no vetor buf. Se o id não estiver bem escrito,
 *      a função retorna -1.
 * 		
 * Entradas: int char*
 *		.
 * Saida: int
 * 		.
 *******************************/
int proccessWITHDRAW(int i, char buf[]){
    int id = -1;

    if(buf[i-1] != ' '){
        return -1;
    }

    if(buf[i+1] == '\n'){
        if(!isdigit(buf[i])){return -1;}

        id = (buf[i] - '0');

    }

    else if(buf[i+2] == '\n'){
        if(!isdigit(buf[i]) || !isdigit(buf[i+1])){return -1;}

        id = (buf[i] - '0')*10 + (buf[i+1] - '0');

    }

    else{
        return -1;
    }

    return id;
}

/*******************************
 * Descrição geral da função:
 * 		Mostra o menu com todas as opções
 *      possíveis.
 * 		
 * Entradas: void
 *		.
 * Saida: void
 * 		.
 *******************************/
void printMenu(void){
    printf("\t\t\t***********************************\n\n"); 
    printf("\t\t\t\t\tMENU\n\n");
    printf("\t\t\t***********************************\n\n\n");
    printf("\tjoin net id - Entrada de um nó com identificador id na rede net\n\n"); 
    printf("\tdjoin net id bootid bootIP bootTCP - Entrada de um nó na rede net\n\tcom identificador id, que se sabe ser único na rede. É passado à\n\taplicação o identificador e o contacto de um nó da rede, através\n\tdos parâmetros bootid, bootIP e bootTCP, ao qual o nó se deverá\n\tligar sem interrogar o servidor de nós\n\n"); 
    printf("\tcreate name - Cria o conteúdo de nome name\n\n"); 
    printf("\tdelete name - Apaga o conteúdo de nome name\n\n"); 
    printf("\tget dest name - Pesquisa do conteúdo com o nome name localizado\n\tno nó dest\n\n"); 
    printf("\tshow topology (st) - Mostra os identificadores e os contactos\n\tdos vizinhos internos, do vizinho externo e do vizinho de\n\trecuperação\n\n"); 
    printf("\tshow names (sn) - Mostra os nomes dos conteúdos presentes no nó\n\n"); 
    printf("\tshow routing (sr) - Mostra a tabela de expedição do nó\n\n");
    printf("\tclear routing (cr) - Apaga a tabela de expedição do nó\n\n");
    printf("\tleave - Saída do nó da rede\n\n\n");
    printf("\texit - Fecho da aplicação\n\n\n");
    printf("\t\t\t***********************************\n\n\n");
    return;
}

/*******************************
 * Descrição geral da função:
 * 		Recebe o número de bits que a chamada anterior a read leu
 *      e lê até ao caracter newline.
 * 		
 * Entradas: void
 *		.
 * Saida: void
 * 		.
 *******************************/
void readFragmentedMessage(char buf[], int fd, int bytes_read){

    int i = 0, j = 0, buffersize = 127;

    while(bytes_read > 0){
        while(bytes_read > (i-j)){
            if(buf[i] == '\n'){
                return;
            }
            ++i;
        }
        buffersize = buffersize - bytes_read;
        bytes_read = TCP_receive(fd, &buf[i], buffersize);
        j = i;
    }

    return;

}