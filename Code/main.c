/*includes */
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

#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#include "dataFunctions.h"
#include "structures.h"
#include "TCP.h"
#include "UDP.h"

#define BUF_SIZE 128
#define STR_SIZE 2700

int main(int argc, char* argv[]){
    // Declaracao de variaveis
    struct timeval timeout;
    fd_set fds, copy_fds;
    int result = 0, flag = 1, instr = 0, net = 0, new_fd = 0, id = 0, reg = 0, unreg = 0, i = 0, nodreg = 0, dest_id = 0, source_id = 0, nodes = 0;
    char buf[BUF_SIZE], IP[20], PORT[20], strNET[20], strID[20], strUDP[STR_SIZE], IPnod[20], PORTnod[20], msg[BUF_SIZE];
    nod* no = NULL;
    intr* intern = NULL, * aux = NULL;
    struct addrinfo *resultados;
    ssize_t n = 0;
    cont* name = NULL;

    // Verificar numero de argumentos
    if(argc != 5 && argc != 3){
        printf("A aplicação não foi bem invocada.\nA forma correta de invocar a aplicação é cot IP TCP (regIP regUDP).\n");
        return 0;
    }

    // Armazenar o endereço IP e o porto do nó nas variáveis IPnod e PORTnod respetivamente
    if(validateIP(argv[1], &id, IPnod) == -1){
        printf("Endereço IP do nó não é válido.\n");
        return -1;
    }
    id = 0;
    
    if(validatePORT(argv[2], &id, PORTnod) == -1){
        printf("Porto do nó não é válido.\n");
        return -1;
    }
    id = 0;

    if(argc == 5){
        if(strcmp(argv[3], "193.136.138.142") != 0){
            printf("O endereço IP não é o endereço IP do servidor de nós.\n");
            return -1;
        }
        if(strcmp(argv[4], "59000") != 0){
            printf("O porto mencionado não é o porto do servidor de nós.\n");
            return -1;
        }
    }

    //printf("Valor de FD_SETSIZE: %d\n", FD_SETSIZE);

    // Inicializacoes

    FD_ZERO(&fds); // Meter todas as entradas do vetor fds a 0
    FD_SET(0,&fds); // Ativar o standard input

    // Menu

    printMenu();

    // Funcao select

    while(flag)
    {
        copy_fds = fds;  //Reinicializar o vetor de ficheiros

        if(reg == 1 || unreg == 1 || nodes == 1){
            if(i == 0){
                // Neste caso só queremos verificar o cliente UDP
                FD_ZERO(&copy_fds);
                FD_SET(no->fd_socket_udp,&copy_fds);
                ++i;
            }
            else if(i == 1){
                // Enviar a mensagem novamente

                printf("Enviou a mensagem para o servidor de nós novamente.\n\n");

                if(reg == 1){ // REG
                    memset(buf, '\0', BUF_SIZE);
		            instr = snprintf(buf, 40, "REG %s %s %s %s", strNET, strID, no->IP, no->PORT);
                    buf[instr] = '\0';

                    n = UDP_send(no->fd_socket_udp, resultados, buf);
                    if(n == -1){
                        printf("Não foi possível enviar a mensagem ao servidor de nós.\n");
                        eliminateNod(&no, resultados);
                        FD_ZERO(&fds);
                        FD_SET(0, &fds);
                        copy_fds = fds;
                        reg = 0;
                        i = 0;
                    }
                }
                else if(nodes == 1){ // NODES
                    memset(buf, '\0', BUF_SIZE);
                    instr = snprintf(buf, 40, "NODES %s", strNET);
                    buf[instr] = '\0';

                    n = UDP_send(no->fd_socket_udp, resultados, buf);
                    if(n == -1){
                        printf("Não foi possível enviar a mensagem ao servidor de nós.\n");
                        eliminateNod(&no, resultados);
                        FD_ZERO(&fds);
                        FD_SET(0, &fds);
                        copy_fds = fds;
                        nodes = 0;
                        i = 0;
                    }
                }
                else{ // UNREG
                    memset(buf, '\0', BUF_SIZE);
		            instr = snprintf(buf, 40, "UNREG %s %s", strNET, strID);
                    buf[instr] = '\0';

                    n = UDP_send(no->fd_socket_udp, resultados, buf);
                    if(n == -1){
                        printf("Não foi possível enviar a mensagem ao servidor de nós.\n");
                        eliminateNod(&no, resultados);
                        FD_ZERO(&fds);
                        FD_SET(0, &fds);
                        copy_fds = fds;
                        unreg = 0;
                        i = 0;
                    }
                }

                // Neste caso só queremos verificar o cliente UDP
                FD_ZERO(&copy_fds);
                FD_SET(no->fd_socket_udp,&copy_fds);
                ++i;
            }
            else{
                i = 0;
                if(reg == 1){
                    printf("Não foi possível registar o nó no servidor de nós.\n");
                    reg = 0;
                    nodreg = 0;
                }
                else if(nodes == 1){
                    printf("Não foi possível obter a lista de nós do servidor de nós.\n");
                    nodes = 0;
                }
                else{
                    printf("Não foi possível retirar o nó do servidor de nós.\n");
                    unreg = 0;
                }
                eliminateNod(&no, resultados);
                FD_ZERO(&fds);
                FD_SET(0, &fds);
                copy_fds = fds;
            }
        }

        // Tempo limite para fazer pedidos
        timeout.tv_sec = 20;
        timeout.tv_usec = 5000;

        // A funcao select está atenta a qualquer alteração num dos ficheiros ativados
        result = select(FD_SETSIZE, &copy_fds, 0, 0, &timeout);

        //printf("Foi ao select\n");

        // Tratamento do resultado de select
        switch(result){

            case 0: // O tempo de espera da aplicação foi ultrapassado
                //printf("Timeout no select.\n");
                break;
            
            case -1: // Erro
                printf("\nOcorreu um erro em select.\n\n");
                exit(1);

            default: // Tratamento do pedido do ficheiro selecionado
                //printf("Entra no switch\n");

                if(FD_ISSET(0,&copy_fds)){
                    //printf("Algo foi introduzido no teclado.\n\n");

                    memset(buf, '\0', BUF_SIZE);
                    // Ler o que está no standard input
                    instr = read(0, buf, BUF_SIZE);

                    // Introduzir um end of string para
                    // que o que foi lido anteriormente não seja considerado
                    buf[instr] = '\0';

                    // Se o numero de bytes lidos não for 0, processar o pedido
                    if(instr != 0){

                        // Funcao que analisa o pedido do utilizador
                        instr = dataHandling(buf);

                        if(instr == 1){ // join
                            no = readDjoin(1, buf, &net, IPnod, PORTnod);
                            if(net != 89){
                                printf("\nA rede tem de ter o valor 89.\n\n");
                                free(no);
                                no = NULL;
                                break;
                            }
                            if(no == NULL){
                                printf("\nErro na leitura.\n\n");
                                FD_ZERO(&fds);
                                FD_SET(0, &fds);
                                break;
                            }

                            if(name != NULL){
                                no->content = name;
                                name = NULL;
                            }

                            // Criar cliente UDP

                            if((no->fd_socket_udp = UDPClientInit(&resultados)) == -1){
                                printf("\nErro a criar o cliente UDP associado ao nó.\n\n");
                                eliminateNod(&no, resultados);
                                FD_ZERO(&fds);
                                FD_SET(0, &fds);
                                continue;
                            }

                            //printf("%d\n", no->fd_socket_udp);

                            // Escrever a mensagem para o servidor de nós - NODES GRUPO

                            memset(buf, '\0', BUF_SIZE);

                            memset(strNET, '\0', 20);

                            if((net - 10) < 0){sprintf(strNET, "00%d", net);}
	                        else if((net - 100) < 0){sprintf(strNET, "0%d", net);}
	                        else{sprintf(strNET, "%d", net);}

                            instr = snprintf(buf, 40, "NODES %s", strNET);
                            buf[instr] = '\0';

                            // Pedir lista de nós ao servidor de nós

                            n = UDP_send(no->fd_socket_udp, resultados, buf);
                            if(n == -1){
                                printf("\nNão foi possível enviar a mensagem ao servidor de nós.\n\n");
                                eliminateNod(&no, resultados);
                                FD_ZERO(&fds);
                                FD_SET(0, &fds);
                                continue;
                            }

                            nodes = 1;

                            printf("\nEnviou-se a mensagem NODES ao servidor\n");

                            FD_SET(no->fd_socket_udp, &fds);

                            break;
                        }
                        else if(instr == 2){ // djoin

                            no = readDjoin(0, buf, &net, IPnod, PORTnod);

                            if(no == NULL){
                                printf("\nErro na leitura.\n\n");
                                break;
                            }

                            if(name != NULL){
                                no->content = name;
                                name = NULL;
                            }

                            if(djoin(no, net) == -1){
                                FD_ZERO(&fds);
                                FD_SET(0, &fds);
                                eliminateNod(&no, resultados);
                                printf("\nErro no djoin.\n\n");
                            }
                            else{
                                printf("Djoin concluído com sucesso.\n\n");
                                FD_SET(no->fd_socket_tcp_server, &fds);
                                if(no->ext->id != no->id){
                                    FD_SET(no->ext->fd_socket_tcp_server, &fds);
                                }
                            }
                        }
                        else if(instr == 3){ // leave
                            if(no == NULL){
                                printf("\nNão é possível libertar um nó que não foi criado.\n\n");
                                break;
                            }
                            if(no->fd_socket_udp == -1){
                                eliminateNod(&no, resultados);
                                FD_ZERO(&fds);
                                FD_SET(0, &fds);
                                printf("\nO nó foi libertado.\n\n");
                            }
                            else{
                                memset(strNET, '\0', 20);

                                if((net - 10) < 0){sprintf(strNET, "00%d", net);}
	                            else if((net - 100) < 0){sprintf(strNET, "0%d", net);}
	                            else{sprintf(strNET, "%d", net);}

                                memset(strID, '\0', 20);
		
		                        if((no->id - 10) < 0){sprintf(strID, "0%d", no->id);}
		                        else{sprintf(strID, "%d", no->id);}

                                // Mandar o comando UNREG ao servidor de nós
                                memset(buf, '\0', BUF_SIZE);
		                        instr = snprintf(buf, 40, "UNREG %s %s", strNET, strID);
                                buf[instr] = '\0';

                                n = UDP_send(no->fd_socket_udp, resultados, buf);
                                if(n == -1){
                                    printf("\nNão foi possível enviar a mensagem ao servidor de nós.\n\n");
                                    eliminateNod(&no, resultados);
                                    continue;
                                }

                                printf("\nEnviou-se a mensagem UNREG ao servidor\n");

                                unreg = 1;
                            }
                        }
                        else if(instr == 4){ // exit
                            if(name != NULL){
                                freeName(&name);
                            }
                            eliminateNod(&no, resultados);
                            flag = 0;
                        }
                        else if(instr == 5){ // show topology
                            id = 0;
                            if(no != NULL){

                                printf("\n");

                                // Nó principal

                                printf("Nó");
                                if(no->id == no->backup->id)
                                {
                                    printf(" âncora");
                                    // se id for 1 então o nó é âncora
                                    id = 1;
                                }
                                printf(":\n\n");
                                printf("id: %d\n", no->id);
                                printf("porto: %s\n", no->PORT);
                                printf("endereço IP: %s\n\n", no->IP);

                                // Nó externo

                                if(no->ext->id != no->id){
                                    printf("Nó");
                                    if(id == 1){
                                        printf(" âncora");
                                    }
                                    else{printf(" externo");}
                                    printf(":\n\n");
                                    printf("id: %d\n", no->ext->id);
                                    printf("porto: %s\n", no->ext->PORT);
                                    printf("endereço IP: %s\n\n", no->ext->IP);
                                }
                                else{
                                    printf("O nó é o único na rede, por isso não tem vizinho externo.\n\n");
                                }

                                // Nó de recuperação

                                if(id == 1){
                                    printf("Nó de recuperação é o próprio nó.\n\n");
                                }
                                else{
                                    printf("Nó de recuperação:\n\n");
                                    printf("id: %d\n", no->backup->id);
                                    printf("porto: %s\n", no->backup->PORT);
                                    printf("endereço IP: %s\n\n", no->backup->IP);
                                }

                                // Nós internos

                                intern = no->inter;
                                while(intern != NULL){
                                    printf("Nó interno:\n\n");
                                    printf("id: %d\n", intern->id);
                                    printf("porto: %s\n", intern->PORT);
                                    printf("endereço IP: %s\n\n", intern->IP);
                                    intern = intern->next_intr;
                                }
                                if(no->id == no->backup->id && no->id != no->ext->id){
                                    printf("Nó interno:\n\n");
                                    printf("id: %d\n", no->ext->id);
                                    printf("porto: %s\n", no->ext->PORT);
                                    printf("endereço IP: %s\n\n", no->ext->IP);
                                }
                                
                            }
                            else{printf("\nNão foram criados quaisquer nós.\n\n");}
                        }
                        else if(instr == 6){ // create name

                            // Função que recebe o vetor de caracteres de stdin
                            // e altera-o para ficar só com o conteúdo name

                            instr = writeContent(buf, 7);
                            if(instr == -1){
                                printf("\nNão foi mencionado nenhum conteúdo.\n\n");
                                break;
                            }

                            if(no != NULL){ // guardar no nó

                                // Função que guarda o conteúdo name

                                instr = createContent(&(no->content), buf);
                                if(instr == -1){
                                    printf("\nNão foi possível criar o conteúdo.\n\n");
                                }
                                else{
                                    printf("\nO conteúdo foi criado.\n\n");
                                }
                            }
                            else{ // guardar numa estrutura auxiliar

                                // Função que guarda o conteúdo name

                                instr = createContent(&name, buf);
                                if(instr == -1){
                                    printf("\nNão foi possível criar o conteúdo.\n\n");
                                }
                                else{
                                    printf("\nO conteúdo foi criado.\n\n");
                                    //printf("%s\n", name->CONTENT);
                                }
                            }
                        }
                        else if(instr == 7){ // delete name

                            // Função que recebe o vetor de caracteres de stdin
                            // e altera-o para ficar só com o conteúdo name

                            instr = writeContent(buf, 7);
                            if(instr == -1){
                                printf("\nNão foi mencionado nenhum conteúdo.\n\n");
                                break;
                            }

                            if(no != NULL){
                                deleteContent(&(no->content), buf);
                            }

                            else{
                                deleteContent(&name, buf);
                            }

                        }
                        else if(instr == 8){ // show names
                            if(no != NULL){
                                readContents(no->content);
                            }

                            else{
                                readContents(name);
                            }
                        }
                        else if(instr == 9){ // get dest name
                            if(no == NULL){
                                printf("\nO nó não se encontra na rede.\n\n");
                                break;
                            }
                            
                            // Processar o comando obtido a partir de stdin

                            result = readGetDest(buf, &id);
                            if(result == -1){
                                printf("\nErro na leitura.\n\n");
                                break;
                            }

                            instr = writeContent(buf, result);
                            if(instr == -1){
                                printf("\nNão foi mencionado nenhum conteúdo.\n\n");
                                break;
                            }

                            // O nó destino é igual ao nó fonte

                            if(id == no->id){
                                // Função que procura o conteúdo

                                if(searchContent(no->content, buf) == -1){
                                    printf("\nO nó %d não possui o conteúdo mencionado.\n\n", no->id);
                                }
                                else{
                                    printf("\nO nó %d possui o conteúdo %s\n\n", no->id, buf);
                                }
                            }
                            else{

                                // Verificar que o nó não está sozinho na rede

                                if(no->id == no->ext->id){
                                    printf("\nO nó está sozinho na rede, por isso não é possível procurar conteúdos num vizinho inexistente.\n\n");
                                    break;
                                }

                                // Analisar a tabela de expedição

                                if(searchExpTable(no, id, no->id, no->id, buf, -1) == -1){
                                    printf("\nOcorreu um erro no envio do comando QUERY.\n\n");
                                    break;
                                }

                            }
                        }
                        else if(instr == 10){ // show routing
                            if(no == NULL){
                                printf("\nNão foi criado nenhum nó, por isso a tabela de expedição é inexistente.\n\n");
                            }
                            else{
                                showRouting(no);
                            }
                        }
                        else if(instr == 11){ // clear routing
                            if(no == NULL){
                                printf("\nNão foi criado nenhum nó, por isso a tabela de expedição é inexistente.\n\n");
                            }
                            else{
                                clearRouting(no);
                                no->expedition_table = NULL;
                            }
                        }
                        else{ // comando invalido
                            printf("\nO comando encontra-se mal escrito ou não é válido.\n\n");
                            break;
                        }
                    }

                }

                // Garantir que o nó foi criado

                else if(no != NULL){

                    if(no->fd_socket_tcp_server != -1){

                    // input no servidor tcp "geral", no "lobby principal"

                    if(FD_ISSET(no->fd_socket_tcp_server, &copy_fds)){

                        // Neste caso, o nó só pode estar a ser contactado
                        // pelo seu vizinho interno

                        //printf("Detetado algo no servidor TCP geral!\n");

                        // Conectar com o cliente
                        new_fd = TCP_Client_Connect(no->fd_socket_tcp_server);
                        if(new_fd == -1){
                            break;
                        }

                        // Receber a mensagem do cliente
                        memset(buf, '\0', BUF_SIZE);
					    instr = TCP_receive(new_fd, buf, BUF_SIZE - 1);
                        if(instr == 0){
                            close(new_fd);
                            printf("\nO nó cliente foi retirado da rede.\n\n");
                            break;
                        }
                        if(instr == -1){break;}
                        readFragmentedMessage(buf, new_fd, instr);
                        //buf[instr] = '\0';

                        // Verificar o comando
                
                        instr = messageCommand(buf, &result);
                        if(instr == -1){
                            printf("O comando da mensagem não foi reconhecido.\n");
                            break;
                        }

                        // Comando NEW
                        if(instr == 1){

                            // Ler o conteúdo da mensagem

                            memset(IP, '\0', 20);
                            memset(PORT, '\0', 20);
                            instr = message(buf, &id, IP, PORT, result);
                            if(instr == -1){
                                printf("A mensagem recebida não está bem escrita.\n");
                                break;
                            }

                            // Se o nó estiver sozinho, atualiza o vizinho externo

                            if(no->ext->id == no->id){
                                if((no->ext = makeExternOrBackup(id, IP, PORT)) == NULL){
                                    break;
                                }

                                // Cliente TCP do nó externo
                                no->ext->fd_socket_tcp_server = new_fd;

                            }

                            else{

                                // Atualizar lista de vizinhos internos

                                intern = no->inter;

                                // Criar novo vizinho interno

                                no->inter = newIntrNeighbour(id, IP, PORT);
                                no->inter->next_intr = intern;

                                // Cliente TCP do vizinho interno
                                no->inter->fd_socket_tcp_server = new_fd;
                            }

                            // Escrever a mensagem EXTERN

                            memset(buf, '\0', BUF_SIZE);
                            result = snprintf(buf, sizeof(buf), "EXTERN %d %s %s\n", no->ext->id, no->ext->IP, no->ext->PORT);
                            if(result == -1){continue;}
                            buf[result] = '\0';

                            // Mandar a mensagem EXTERN ao cliente

                            if(TCP_send(new_fd, buf)==-1){
                                printf("Houve um erro a enviar a mensagem.\n");
                            }

                            FD_SET(new_fd, &fds);

                        }

                        else{
                            printf("O comando fornecido não pode ser executado.\n");
                        }

                        break;
                    }
                    }

                    if(no->fd_socket_udp != -1){
                        //input no cliente UDP

                        //printf("Entrou no cliente UDP\n");

				        if(FD_ISSET(no->fd_socket_udp, &copy_fds)){

                            printf("Está a receber algo do servidor de nós\n");

                            memset(strUDP, '\0', STR_SIZE);

                            // Receber mensagem do servidor de nós
                            n = UDP_receive(no->fd_socket_udp, resultados, strUDP);

                            if(n == -1){
                                printf("Não foi possível ler do servidor de nós.\n");
                                if(no != NULL){
                                    i = 0;
                                    nodes = 0;
                                    reg = 0;
                                    unreg = 0;
                                    nodreg = 0;
                                    eliminateNod(&no, resultados);
                                    FD_ZERO(&fds);
                                    FD_SET(0, &fds);
                                    break;
                                }
                            }

                            strUDP[n] = '\0';

                            //printf("%s\n", strUDP);

                            // Processar mensagem do servidor de nós

                            if((instr = messageUDP(strUDP, no, net)) == -1){
                                i = 0;
                                nodes = 0;
                                reg = 0;
                                unreg = 0;
                                nodreg = 0;
                                eliminateNod(&no, resultados);
                                FD_ZERO(&fds);
                                FD_SET(0, &fds);
                                break;
                            }

                            if(instr == 1){
                                nodes = 0;
                                i = 0;

                                FD_SET(no->fd_socket_tcp_server, &fds);

                                if(no->ext->id != no->id){
                                    FD_SET(no->ext->fd_socket_tcp_server, &fds);
                                }

                                else{ // nó está sozinho

                                    memset(strNET, '\0', 20);

                                    if((net - 10) < 0){sprintf(strNET, "00%d", net);}
	                                else if((net - 100) < 0){sprintf(strNET, "0%d", net);}
	                                else{sprintf(strNET, "%d", net);}

                                    memset(strID, '\0', 20);
		
		                            if((no->id - 10) < 0){sprintf(strID, "0%d", no->id);}
		                            else{sprintf(strID, "%d", no->id);}

                                    // Escrever a mensagem no buffer

                                    memset(buf, '\0', BUF_SIZE);
		                            instr = snprintf(buf, 40, "REG %s %s %s %s", strNET, strID, no->IP, no->PORT);
                                    buf[instr] = '\0';

                                    // Enviar mensagem ao servidor de nós

                                    n = UDP_send(no->fd_socket_udp, resultados, buf);
                                    if(n == -1){
                                        printf("Não foi possível enviar a mensagem ao servidor de nós.\n");
                                        eliminateNod(&no, resultados);
                                        FD_ZERO(&fds);
                                        FD_SET(0, &fds);
                                        break;
                                    }

                                    printf("Enviou-se a mensagem REG ao servidor de nós.\n");

                                    reg = 1;
                                }
                            }

                            if(instr == 2){
                                reg = 0;
                                nodreg = 1;
                                i = 0;
                            }

                            if(instr == 3){
                                unreg = 0;
                                nodreg = 0;
                                i = 0;

                                // Retirar o nó da rede
                                eliminateNod(&no, resultados);
                                FD_ZERO(&fds);
                                FD_SET(0, &fds);
                            }

                            break;
                        }
                    }

                    if(no->ext != NULL && no->ext->id != no->id && no->ext->fd_socket_tcp_server != -1){
                        //input no servidor tcp do vizinho externo

                        if(FD_ISSET(no->ext->fd_socket_tcp_server, &copy_fds)){

                            // Receber a mensagem do servidor do nó externo

                            memset(buf, '\0', BUF_SIZE);
					        instr = TCP_receive(no->ext->fd_socket_tcp_server, buf, BUF_SIZE - 1);
                            if(instr == -1){break;}
                            if(instr == 0){
                                FD_CLR(no->ext->fd_socket_tcp_server, &fds);
                                close(no->ext->fd_socket_tcp_server);

                                if(freeExpTableEntries(no, no->ext->id, no->ext->id) == -1){
                                    printf("Não foi possível eliminar as entradas na tabela de expedição que incluiam o nó que foi retirado.\n");
                                }

                                usleep(250);

                                // Caso em que o nó não é âncora
                                if(no->id != no->backup->id){

                                    no->ext->id = no->backup->id;
                                    memset(no->ext->IP, '\0', 20);
                                    strcpy(no->ext->IP, no->backup->IP);
                                    memset(no->ext->PORT, '\0', 20);
                                    strcpy(no->ext->PORT, no->backup->PORT);

                                    // Criar sessão TCP do nó externo

                                    if((no->ext->fd_socket_tcp_server = TCP_Client_Init(no->ext->IP, no->ext->PORT)) == -1){
                                        printf("Não foi possível comunicar com o nó cujo id é %d.\n", no->ext->id);
                                        eliminateNod(&no, resultados);
                                        break;
                                    }

                                    FD_SET(no->ext->fd_socket_tcp_server, &fds);

                                    // Enviar mensagem NEW ao vizinho externo
                                    instr = snprintf(buf, sizeof(buf), "NEW %d %s %s\n", no->id, no->IP, no->PORT);
                                    buf[instr] = '\0';

                                    if(TCP_send(no->ext->fd_socket_tcp_server, buf)==-1){
                                        printf("Ocorreu um problema a enviar o comando NEW.\n");
                                        // Será que deveria eliminar o nó da rede?
                                        eliminateNod(&no, resultados);
                                        break;
                                    }

                                    // Envia mensagem EXTERN a todos os vizinhos internos

                                    intern = no->inter;
                                    while(intern != NULL){

                                        // Escrever a mensagem EXTERN

                                        memset(buf, '\0', BUF_SIZE);
                                        result = snprintf(buf, sizeof(buf), "EXTERN %d %s %s\n", no->ext->id, no->ext->IP, no->ext->PORT);
                                        if(result == -1){continue;}
                                        buf[result] = '\0';

                                        // Mandar a mensagem EXTERN ao vizinho interno atual

                                        if(TCP_send(intern->fd_socket_tcp_server, buf)==-1){
                                            printf("Houve um erro a enviar a mensagem EXTERN.\n");
                                        }

                                        // Vizinho interno seguinte

                                        intern = intern->next_intr;
                                    }

                                }

                                // Caso em que o nó é âncora
                                else{
                                    intern = no->inter;

                                    if(intern != NULL){ // o nó não ficou sozinho na rede

                                        // O vizinho interno escolhido vai passar a ser também vizinho
                                        // externo do nó
                                        no->ext->id = intern->id;
                                        memset(no->ext->IP, '\0', 20);
                                        strcpy(no->ext->IP, intern->IP);
                                        memset(no->ext->PORT, '\0', 20);
                                        strcpy(no->ext->PORT, intern->PORT);
                                        no->ext->fd_socket_tcp_server = intern->fd_socket_tcp_server;

                                        aux = no->inter;

                                        while(aux != NULL){

                                        // Escrever a mensagem EXTERN

                                            memset(buf, '\0', BUF_SIZE);
                                            result = snprintf(buf, sizeof(buf), "EXTERN %d %s %s\n", no->ext->id, no->ext->IP, no->ext->PORT);
                                            if(result == -1){break;}
                                            buf[result] = '\0';

                                            // Mandar a mensagem EXTERN ao vizinho interno atual

                                            if(TCP_send(aux->fd_socket_tcp_server, buf)==-1){
                                                printf("Houve um erro a enviar a mensagem EXTERN.\n");
                                            }

                                            // Vizinho interno seguinte

                                            aux = aux->next_intr;
                                        }

                                        no->inter = intern->next_intr;
                                        free(intern);
                                    }
                                    else{ // o nó ficou sozinho na rede
                                        no->ext->id = no->id;
                                        memset(no->ext->IP, '\0', 20);
                                        strcpy(no->ext->IP, no->IP);
                                        memset(no->ext->PORT, '\0', 20);
                                        strcpy(no->ext->PORT, no->PORT);
                                        no->ext->fd_socket_tcp_server = -1;
                                    }

                                }

                                printf("O nó externo foi retirado da rede.\n");

                                break;
                            }

                            readFragmentedMessage(buf, no->ext->fd_socket_tcp_server, instr);

                            //buf[instr] = '\0';

                            // Aferir qual é a mensagem recebida

                            instr = messageCommand(buf, &result);
                            if(instr == -1){
                                printf("O comando da mensagem não foi reconhecido.\n");
                                break;
                            }

                            // Comando EXTERN

                            if(instr == 2){

                                // Processar a mensagem

                                memset(IP, '\0', 20);
                                memset(PORT, '\0', 20);
                                instr = message(buf, &id, IP, PORT, result);
                                if(instr == -1){
                                    printf("A mensagem recebida não está bem escrita.\n");
                                    break;
                                }

                                printf("Criou o vizinho de recuperação.\n");

                                if(id == no->id){
                                    no->backup = makeExternOrBackup(id, no->IP, no->PORT);
                                }

                                else if(no->backup == NULL){
                                    no->backup = makeExternOrBackup(id, IP, PORT);
                                }

                                else{
                                    no->backup->id = id;
                                    memset(no->backup->IP, '\0', 20);
                                    strcpy(no->backup->IP, IP);
                                    memset(no->backup->PORT, '\0', 20);
                                    strcpy(no->backup->PORT, PORT);
                                }

                                // Só é criado um cliente UDP se o nó for criado através
                                // do comando join
                                if(no->fd_socket_udp != -1 && nodreg == 0){

                                    // Registar o nó no servidor de nós
                                    
                                    if(no == NULL){
			                            printf("Nó a registar não existe.\n");
			                            continue;
		                            }

                                    // IP - net
                                    // PORT - id

                                    memset(strNET, '\0', 20);

                                    if((net - 10) < 0){sprintf(strNET, "00%d", net);}
	                                else if((net - 100) < 0){sprintf(strNET, "0%d", net);}
	                                else{sprintf(strNET, "%d", net);}

                                    memset(strID, '\0', 20);
		
		                            if((no->id - 10) < 0){sprintf(strID, "0%d", no->id);}
		                            else{sprintf(strID, "%d", no->id);}

                                    // Escrever a mensagem no buffer

                                    memset(buf, '\0', BUF_SIZE);
		                            instr = snprintf(buf, 40, "REG %s %s %s %s", strNET, strID, no->IP, no->PORT);
                                    buf[instr] = '\0';

                                    // Mandar a mensagem ao servidor de nós

                                    n = UDP_send(no->fd_socket_udp, resultados, buf);
                                    if(n == -1){
                                        printf("Não foi possível enviar a mensagem ao servidor de nós.\n");
                                        eliminateNod(&no, resultados);
                                        continue;
                                    }

                                    printf("Enviou-se a mensagem REG ao servidor de nós.\n");

                                    reg = 1;
                                }

                            }

                            // Comando QUERY

                            else if(instr == 3){

                                // Função que processa a mensagem QUERY

                                if(proccessCommand(buf, &dest_id, &source_id, result) == -1){
                                    printf("A mensagem QUERY não foi bem escrita.\n");
                                    break;
                                }

                                // Verificar se este é o nó de destino

                                if(no->id == dest_id){

                                    // Procurar o conteúdo mencionado

                                    if(searchContent(no->content, buf) == -1){

                                        // O conteúdo não foi encontrado
                                        memset(msg, '\0', BUF_SIZE);

                                        strcpy(msg, buf);

                                        // Criar a mensagem
                                        instr = snprintf(buf, sizeof(buf), "NOCONTENT %d %d %s\n", source_id, dest_id, msg);
                                        if(instr < 0){
                                            printf("Não foi possível criar a mensagem NOCONTENT.\n");
                                            break;
                                        }
                                        buf[instr] = '\0';

                                        // Enviar a mensagem NOCONTENT
                                        if(TCP_send(no->ext->fd_socket_tcp_server, buf)==-1){
                                            printf("Não foi possível enviar a mensagem NOCONTENT.\n");
                                            break;
                                        }

                                        //printf("%s\n", buf);

                                    }
                                    else{
                                        // O conteúdo foi encontrado

                                        memset(msg, '\0', BUF_SIZE);

                                        strcpy(msg, buf);

                                        // Criar a mensagem
                                        instr = snprintf(buf, sizeof(buf), "CONTENT %d %d %s\n", source_id, dest_id, msg);
                                        if(instr < 0){
                                            printf("Não foi possível criar a mensagem CONTENT.\n");
                                            break;
                                        }
                                        buf[instr] = '\0';

                                        // Enviar a mensagem CONTENT
                                        if(TCP_send(no->ext->fd_socket_tcp_server, buf)==-1){
                                            printf("Não foi possível enviar a mensagem CONTENT.\n");
                                            break;
                                        }

                                        //printf("%s\n", buf);
                                    }

                                    // Verificar que não há nenhuma entrada em que o nó
                                    // onde se originou a mensagem é o nó destino

                                    if(fillDestExpTable(no, source_id, no->ext->id, no->ext->fd_socket_tcp_server) == -1){
                                        break;
                                    }

                                }

                                // Se não for analisa-se a tabela de expedição

                                else{
                                    if(searchExpTable(no, dest_id, no->ext->id, source_id, buf, no->ext->fd_socket_tcp_server) == -1){
                                        printf("Ocorreu um erro no envio do comando QUERY.\n");
                                    }
                                }

                            }

                            // Comando CONTENT ou NOCONTENT

                            else if((instr == 4) || (instr == 5)){

                                // Função que processa a mensagem CONTENT ou NOCONTENT

                                if(proccessCommand(buf, &dest_id, &source_id, result) == -1){
                                    printf("\nA mensagem não foi bem escrita.\n");
                                    break;
                                }

                                if(no->id == dest_id){
                                    if(instr == 4){ // O conteúdo existe
                                        printf("\nO nó %d possui o conteúdo %s\n\n", source_id, buf);
                                    }
                                    else{ // O conteúdo não existe
                                        printf("\nO nó %d não possui o conteúdo %s\n\n", source_id, buf);
                                    }

                                    // Preencher a tabela de expedição

                                    if(fillDestExpTable(no, source_id, no->ext->id, no->ext->fd_socket_tcp_server) == -1){
                                        break;
                                    }
                                }

                                else{
                                    // Procurar a entrada na tabela de expedição cujo ID do
                                    // destino equivale a dest_id

                                    if(contentExpTable(no, source_id, dest_id, no->ext->id, buf, instr, no->ext->fd_socket_tcp_server) == -1){
                                        if(instr == 4){
                                            printf("Não foi possível enviar a mensagem CONTENT.\n");
                                        }
                                        else{
                                            printf("Não foi possível enviar a mensagem NOCONTENT.\n");
                                        }
                                        break;
                                    }
                                }

                            }

                            // Comando WITHDRAW

                            else if(instr == 6){

                                // Processar a mensagem WITHDRAW

                                if((source_id = proccessWITHDRAW(result, buf)) == -1){
                                    printf("A mensagem não se encontra bem escrita.\n");
                                    break;
                                }

                                // Eliminar a entrada cujo parâmetro destino é o ID do nó retirado
                                // Se esta entrada existe, manda-se a mensagem WITHDRAW para todos
                                // os vizinhos excepto aquele pelo qual se recebeu a mensagem

                                if(freeExpTableEntries(no, source_id, no->ext->id) == -1){
                                    printf("Não foi possível eliminar as entradas na tabela de expedição que incluiam o nó que foi retirado.\n");
                                }
                            }

                            else{
                                printf("O comando fornecido não pode ser executado.\n");
                            }

                            break;
                        }
                    }

                    //input no servidor tcp dos vizinhos internos
                    intern = no->inter;
                    aux = NULL;
                    instr = -1;

                    // Garante que o nó interno não é também o nó externo
                    // Caso em que o nó é âncora
                    while(intern != NULL){

                        if(FD_ISSET(intern->fd_socket_tcp_server, &copy_fds)){

                            memset(buf, '\0', BUF_SIZE);
					        instr = TCP_receive(intern->fd_socket_tcp_server, buf, BUF_SIZE - 1);
                            if(instr == -1){break;}
                            if(instr == 0){
                                FD_CLR(intern->fd_socket_tcp_server, &fds);
                                close(intern->fd_socket_tcp_server);

                                if(freeExpTableEntries(no, intern->id, intern->id) == -1){
                                    printf("Não foi possível eliminar as entradas na tabela de expedição que incluiam o nó que foi retirado.\n");
                                }

                                if(aux == NULL){
                                    no->inter = intern->next_intr;
                                    free(intern);
                                }
                                else{
                                    aux->next_intr = intern->next_intr;
                                    free(intern);
                                }
                                
                                printf("O nó interno foi retirado da rede.\n");
                                break;
                            }

                            readFragmentedMessage(buf, intern->fd_socket_tcp_server, instr);

                            // Aferir qual é a mensagem recebida

                            instr = messageCommand(buf, &result);
                            if(instr == -1){
                                printf("O comando da mensagem não foi reconhecido.\n");
                                break;
                            }

                            // Comando QUERY

                            if(instr == 3){

                                // Função que processa a mensagem QUERY

                                if(proccessCommand(buf, &dest_id, &source_id, result) == -1){
                                    printf("A mensagem QUERY não foi bem escrita.\n");
                                    break;
                                }

                                // Verificar se este é o nó de destino

                                if(no->id == dest_id){

                                    // Procurar o conteúdo mencionado

                                    if(searchContent(no->content, buf) == -1){

                                        // O conteúdo não foi encontrado

                                        memset(msg, '\0', BUF_SIZE);

                                        strcpy(msg, buf);

                                        // Criar a mensagem
                                        instr = snprintf(buf, sizeof(buf), "NOCONTENT %d %d %s\n", source_id, dest_id, msg);
                                        if(instr < 0){
                                            printf("Não foi possível criar a mensagem NOCONTENT.\n");
                                            break;
                                        }
                                        buf[instr] = '\0';

                                        // Enviar a mensagem NOCONTENT
                                        if(TCP_send(intern->fd_socket_tcp_server, buf)==-1){
                                            printf("Não foi possível enviar a mensagem NOCONTENT.\n");
                                            break;
                                        }

                                        //printf("%s\n", buf);

                                    }
                                    else{

                                        // O conteúdo foi encontrado

                                        memset(msg, '\0', BUF_SIZE);

                                        strcpy(msg, buf);

                                        // Criar a mensagem
                                        instr = snprintf(buf, sizeof(buf), "CONTENT %d %d %s\n", source_id, dest_id, msg);
                                        if(instr < 0){
                                            printf("Não foi possível criar a mensagem CONTENT.\n");
                                            break;
                                        }
                                        buf[instr] = '\0';

                                        // Enviar a mensagem CONTENT
                                        if(TCP_send(intern->fd_socket_tcp_server, buf)==-1){
                                            printf("Não foi possível enviar a mensagem CONTENT.\n");
                                            break;
                                        }

                                        //printf("%s\n", buf);
                                    }

                                    // Verificar que não há nenhuma entrada em que o nó
                                    // onde se originou a mensagem é o nó destino

                                    if(fillDestExpTable(no, source_id, intern->id, intern->fd_socket_tcp_server) == -1){
                                        break;
                                    }

                                }

                                // Se não for analisa-se a tabela de expedição

                                else{
                                    if(searchExpTable(no, dest_id, intern->id, source_id, buf, intern->fd_socket_tcp_server) == -1){
                                        printf("Ocorreu um erro no envio do comando QUERY.\n");
                                    }
                                }

                            }

                            // Comando CONTENT ou NOCONTENT

                            else if((instr == 4) || (instr == 5)){

                                // Função que processa a mensagem CONTENT ou NOCONTENT

                                if(proccessCommand(buf, &dest_id, &source_id, result) == -1){
                                    printf("A mensagem não foi bem escrita.\n");
                                    break;
                                }

                                if(no->id == dest_id){
                                    if(instr == 4){ // O conteúdo existe
                                        printf("\nO nó %d possui o conteúdo %s\n\n", source_id, buf);
                                    }
                                    else{ // O conteúdo não existe
                                        printf("\nO nó %d não possui o conteúdo %s\n\n", source_id, buf);
                                    }

                                    // Preencher a tabela de expedição

                                    if(fillDestExpTable(no, source_id, intern->id, intern->fd_socket_tcp_server) == -1){
                                        break;
                                    }
                                }

                                else{
                                    // Procurar a entrada na tabela de expedição cujo ID do
                                    // destino equivale a dest_id

                                    if(contentExpTable(no, source_id, dest_id, intern->id, buf, instr, intern->fd_socket_tcp_server) == -1){
                                        if(instr == 4){
                                            printf("Não foi possível enviar a mensagem CONTENT.\n");
                                        }
                                        else{
                                            printf("Não foi possível enviar a mensagem NOCONTENT.\n");
                                        }
                                        break;
                                    }
                                }

                            }

                            // Comando WITHDRAW

                            else if(instr == 6){

                                // Processar a mensagem WITHDRAW

                                if((source_id = proccessWITHDRAW(result, buf)) == -1){
                                    printf("A mensagem não se encontra bem escrita.\n");
                                    break;
                                }

                                // Eliminar a entrada cujo parâmetro destino é o ID do nó retirado
                                // Se esta entrada existe, manda-se a mensagem WITHDRAW para todos
                                // os vizinhos excepto aquele pelo qual se recebeu a mensagem

                                if(freeExpTableEntries(no, source_id, intern->id) == -1){
                                    printf("Não foi possível eliminar as entradas na tabela de expedição que incluiam o nó que foi retirado.\n");
                                }
                            }

                            else{
                                printf("O comando fornecido não pode ser executado.\n");
                            }

                            break;
                        }

                        aux = intern;
                        intern = intern->next_intr;
                    }
                }

        }

    }

}