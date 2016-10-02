#include "simulator.h"



void init(struct nodo *);

int main(int argc, char *argv[])
{
	struct nodo *prova;
	prova = (struct nodo *)malloc( sizeof(struct nodo));
	
	printf("1 dimensione int = %d \n" , sizeof(int)) ;
	printf("1 dimensione time_t = %d \n" , sizeof(time_t)) ;						                             
	printf("1 dimensione nodo = %d \n" , sizeof(*prova)) ;

	

   return 0;
}

