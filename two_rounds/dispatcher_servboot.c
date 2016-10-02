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

	addr_init(&servaddr,PORT3,INADDR_ANY);


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
	sem_post(sem_init_servboot);
	
	sem_wait(sem_exit_servboot);

	sem_wait(exit_hdisp_boot);
		exit_hboot = 1;
	sem_post(exit_hdisp_boot);	

	sleep(_TIME_HELPER);
	for(t=0;t < THREADS_BOOT;t++)
	{
  		pthread_cancel(threads[t] );
	}			
	close(listensd);
	pthread_exit(NULL);
}





void *func_helper_servboot(void *l)
{	
	int listensd = (int)l ;
	int                 connsd;
	Gossip_Server_message *buff;	

	if(_DEBUG)
		printf("Creating thread_servboot %lu\n", (unsigned long)pthread_self());	

	for ( ; ; ) 
	{

		short int exit_helper;
		sem_wait(exit_hdisp_boot);
			exit_helper = exit_hboot;
		sem_post(exit_hdisp_boot);
		
		if(exit_helper)
				pthread_exit(NULL);	

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

		if(_DEBUG)
		{
			stampa_rcv_Gossip_servMsg(buff);
		}

		free(buff);
		close(connsd);
		
	}
	pthread_exit(NULL);
}

