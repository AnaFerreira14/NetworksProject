
CC = gcc
CFLAGS = -Wall -std=c99 -O3 -D_GNU_SOURCE

OBJETOS = main.o dataFunctions.o TCP.o UDP.o #structures.o

cot: $(OBJETOS)
	$(CC) $(CFLAGS) -o $@ $(OBJETOS)

main.o: main.c dataFunctions.h structures.h TCP.h UDP.h

dataFunctions.o: dataFunctions.c dataFunctions.h structures.h TCP.h UDP.h

TCP.o: TCP.c TCP.h structures.h

UDP.o: UDP.c UDP.h structures.h dataFunctions.h

clean:
	rm -f *.o  cot
	
	
VALG = valgrind --leak-check=full
FILES = $(shell ls ...) #mudar o path para os ficheiros a testar

#caso seja necessário, exemplo de como correr testes:
tv_all: 
	for F in ${FILES}; do ${VALG} ./backbone $${F} ; done;
	

