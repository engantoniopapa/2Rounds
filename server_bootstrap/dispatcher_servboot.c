#include "header.h"


void *func_dispatcher_servboot()
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

	addr_init(&servaddr,_PORT_SERVER,INADDR_ANY);


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
	printf("Creating disp_servboot %lu\n", (unsigned long)pthread_self());		
	pthread_t threads[THREADS_BOOT];

	

	for(t=0;t < THREADS_BOOT;t++)
	{
		int rc ;

  	rc = pthread_create (&threads[t], NULL, func_helper_servboot, (void *)listensd);
  	if (rc)
		{
     	printf("ERROR; return code from pthread_create() is %d\n",rc);
     	exit(-1);
  	}
	}

	
	for(t=0;t < THREADS_BOOT;t++)
	{
  	pthread_join(threads[t] , NULL);
	}				
	close(listensd);
	pthread_exit(NULL);
}





void *func_helper_servboot(void *l)
{	
	int listensd = (int)l ;
	int                 connsd;
	Gossip_Server_message *buff;	
	Gossip_Server_nodi *msg_nodi;

	printf("Creating thread_servboot %lu\n", (unsigned long)pthread_self());	

	for ( ; ; ) 
	{
		sem_wait(sem_accept_servboot);

			if ((connsd = accept(listensd, (struct sockaddr *) NULL, NULL)) < 0) 
			{
	 			perror("errore in accept");
	 			exit(1);
	 		}
		sem_post(sem_accept_servboot);
	

		// Alloco memoria per ricevere il messaggio
    buff = (Gossip_Server_message*) malloc(sizeof(Gossip_Server_message));
 
    // Ricevo il gossip_message
    recv(connsd,buff,sizeof(Gossip_Server_message),0);
		
		short int r1 ; 
		
		if(buff->header.pay_desc == _BOOT_SERVER_PING)
		{

			r1 = search_nodo(buff->port ,  buff->ip);
			
			if(r1 == 0)
			{ 
				iscrizione(buff->port ,  buff->ip );	
			}		
			
				msg_nodi = init_Gserver_nodi();
				set_Gserver_nodi( msg_nodi , buff->port ,  buff->ip);
				send(connsd,msg_nodi,sizeof(Gossip_Server_nodi),0);
				free(msg_nodi);


		}
		free(buff);
		close(connsd);
		
	}
	pthread_exit(NULL);
}

