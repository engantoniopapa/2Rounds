#include "header.h"


void bootstrap();

int main(int argc, char *argv[])
{

	srand(time(NULL));

	bootstrap();
							                             


   return 0;
}



void bootstrap()
{
	my_port =  ( in_port_t *) malloc (sizeof(in_port_t )); // inizializzazione propria porta in network order
	*my_port = htons(_PORT_SERVER);

	my_ip =  ( struct in_addr *) malloc (sizeof(struct in_addr)); // inizializzazione proprio ip in network order
	char string_ip[16] ;
	printf("Inserisci l'ip :");
	scanf("%s" , string_ip);
	printf("\n");
	if (inet_pton(AF_INET, string_ip , my_ip) <= 0) 
	{
  	fprintf(stderr, "errore in inet_pton per %s", string_ip);
  	exit(1);
  }
	printf("ip : %u \n" , my_ip->s_addr );

	id_msg = init_id() ; //inizializzo l'id message

	buffer_nodi = init_buffer_nodi();

	sem_buf_nodi = make_Writers_Reader();
	sem_accept_servboot =  make_semaphore(1) ;	// mutex per il lock sull'accept

	pthread_t dispatcher;

	int rc ;
	printf("Fine bootstrap, avvio disptacher \n");
	rc = pthread_create (&dispatcher, NULL, func_dispatcher_servboot, (void *)NULL);
	if (rc)
	{
   	printf("ERROR; return code from pthread_create() is %d\n",rc);
   	exit(-1);
	}

	pthread_join(dispatcher , NULL);
}
