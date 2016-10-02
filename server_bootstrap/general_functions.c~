#include "header.h"

char *init_id() // inizializza l'id message per i messaggi server_boot
{
	char *id;
	id = (char *) malloc(sizeof(char) * _ID_M );
	
	int i;
	for(i = 0 ; i < _ID_M ; ++i)
			id[i] = 0;
	return id;
}


int num_random(int num) // genera un numero random da 0 a num-1
{

	int a;
  a =(int)( num * (rand()/(RAND_MAX+1.0)));
  return a;

}


// Inizializzazione della variabile sockaddr_in
void addr_init(struct sockaddr_in *addr, int port, long int ip)  
{
        addr->sin_family=AF_INET;
        addr->sin_port = htons ((u_short) port);
        addr->sin_addr.s_addr=ip;
}



