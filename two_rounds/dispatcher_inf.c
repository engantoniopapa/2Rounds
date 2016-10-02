#include "header.h"



void *func_dispatcher_inf()
{
	int listensd ;
  struct sockaddr_in servaddr ;
  int t ;

	if ((listensd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  {
  	perror("errore in socket");
  	exit(1); 
  }

	memset((char *)&servaddr, 0, sizeof(servaddr));

	addr_init(&servaddr,PORT2,INADDR_ANY);


	if (bind(listensd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) 
  {
  	perror("errore in bind");
 		exit(1); 
  }

  if (listen(listensd, BACKLOG) < 0 ) 
  {
  	perror("errore in listen");
  	exit(1);
  }
		if(_DEBUG)
			printf("Creating disp_inf %lu\n", (unsigned long)pthread_self());		
		
		pthread_t threads[THREADS_INF];
	
		for(t=0;t < THREADS_INF;t++)
		{
			int rc ;
	
    	rc = pthread_create (&threads[t], NULL, func_helper_inf, (void *)listensd);
    	if (rc)
			{
       	printf("ERROR; return code from pthread_create() is %d\n",rc);
       	exit(-1);
    	}
  	}
		sem_post(sem_init_inf);
	
		sem_wait(sem_exit_inf);		

		sem_wait(exit_hdisp_inf);
			exit_hinf = 1;
		sem_post(exit_hdisp_inf);		

		sleep(_TIME_HELPER);
		for(t=0;t < THREADS_INF;t++)
		{
  		pthread_cancel(threads[t]);
		}		
			
		close(listensd);
		pthread_exit(NULL);
}


void *func_helper_inf(void *l)
{	
	int listensd = (int)l ;
	int                 connsd;
	int len;
	struct sockaddr_in cliaddr;
	Gossip_informations2 *buff;	

	if(_DEBUG)
		printf("Creating thread_inf %lu\n", (unsigned long)pthread_self());		
	
	for ( ; ; ) 
	{
		short int exit_helper;
		sem_wait(exit_hdisp_inf);
			exit_helper = exit_hinf;
		sem_post(exit_hdisp_inf);
		
		if(exit_helper)
				pthread_exit(NULL);
		
		len = sizeof(struct sockaddr_in);
		memset((char *)&cliaddr, 0, sizeof(struct sockaddr_in));
		
		sem_wait(sem_accept_inf);
			if ((connsd = accept(listensd, (struct sockaddr*)&cliaddr, (socklen_t *)&len)) < 0) 
			{
   			perror("errore in accept");
   			exit(1);
 			}
		sem_post(sem_accept_inf);
		
		// Alloco memoria per ricevere la stringa
    buff = (Gossip_informations2*) malloc(sizeof(Gossip_informations2));

    // Ricevo il gossip_information
    recv(connsd,buff,sizeof(Gossip_informations2),0);

		if(_DEBUG)
		{
			stampa_rcv_Gossip_inf(buff);
		}

		insert_sorgente_gossipInf( buffer_neighbour , buff->port  , buff->ip  , buff->hash_file) ;
		
		
		int i ;
		for(i = 0 ; i < _GOSSIP_NODI ;++i)
		{	
			if(buff->list_neighbour[i].port == 0 && buff->list_neighbour[i].ip.s_addr == 0)
			{
				i = _GOSSIP_NODI;
			}
			else
			{
				buff->list_neighbour[i].age++ ;
				
				int age = buff->list_neighbour[i].age;
				insert_peer_gossipInf(buffer_neighbour , age , buff->list_neighbour[i].port , buff->list_neighbour[i].ip , buff->list_neighbour[i].hash_file) ;
			}
		}

		free(buff);

		close(connsd);
	}
	pthread_exit(NULL);
}




